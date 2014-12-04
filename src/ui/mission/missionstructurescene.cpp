#include "missionstructurescene.h"

const qreal TextRectItem::PADDING = 10;
const qreal TextRectItem::LINE_SPACING = 1.5f;
TextRectItem::TextRectItem()
{
    rect = new QGraphicsRectItem();
    lines = QList<QString>();
    graphicLines = QList<QGraphicsSimpleTextItem*>();

    textBrush = QBrush();
    rectBrush = QBrush();
}

TextRectItem::TextRectItem(QList<QString> textLines, QFont font) :
    TextRectItem()
{
    rect = new QGraphicsRectItem();
    lines = textLines;

    for(QString line : lines)
    {
        // Create the new item and adjust height/width of rect
        QGraphicsSimpleTextItem* currentItem = new QGraphicsSimpleTextItem(line, rect);

        // Set font
        currentItem->setFont(font);

        // Add the new item
        graphicLines.append(currentItem);
    }

    updateLinePositions();
    updateRect();
}

TextRectItem::~TextRectItem()
{

}

void TextRectItem::updateRect()
{
    QRectF theRect = rect->childrenBoundingRect();
    theRect.setWidth(theRect.width()+PADDING);
    theRect.setHeight(theRect.height()+PADDING);
    rect->setRect(theRect);
}

void TextRectItem::updateLinePositions()
{
    for(int i = 0; i < graphicLines.length(); i++)
        graphicLines[i]->setPos(0, (graphicLines[i]->boundingRect().height()*LINE_SPACING) * i);
}

void TextRectItem::addLine(QString line, QFont font)
{
    QGraphicsSimpleTextItem* item = new QGraphicsSimpleTextItem(line, rect);
    item->setFont(font);
    item->setPos(0, (item->boundingRect().height()*LINE_SPACING) * graphicLines.length());
    graphicLines.append(item);
    updateRect();
}

void TextRectItem::addLineAt(int index, QString line, QFont font)
{
    if(index < 0)
        index = 0;

    QGraphicsSimpleTextItem* item = new QGraphicsSimpleTextItem(line, rect);
    item->setFont(font);

    if(index > graphicLines.length())
        graphicLines.append(item);
    else
        graphicLines.insert(index, item);

    updateLinePositions();
    updateRect();
}

void TextRectItem::removeLine(QString line)
{

}

QGraphicsRectItem* TextRectItem::getRect()
{
    return rect;
}

QList<QGraphicsSimpleTextItem*> TextRectItem::getGraphicLines()
{
    return graphicLines;
}

QGraphicsSimpleTextItem* TextRectItem::getGraphicLineAt(int index)
{
    return graphicLines.at(index);
}

MissionStructureScene::MissionStructureScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void MissionStructureScene::setMission(Mission* mission)
{
    this->mission = mission;
    QObject::connect(mission->getObserver(), SIGNAL(updated()), this, SLOT(missionUpdated()));
    missionUpdated();
}

void MissionStructureScene::missionUpdated()
{
    // Clear existing data
    clear();

    // Re-create the lists (as clear has deleted all graphics items)
    stages = QList<TextRectItem>();
    gates = QList<TextRectItem>();

    QList<Stage*> missionStages = mission->getStages();

    qreal tallest = 0.f;

    // Construct lists of stages and gates
    for(Stage* stage : missionStages)
    {
        QList<QString> keyNames; // Names of keys in this stage
        QList<Key*> keys = stage->getKeys(); // List of keys in this stage
        for(Key* key : keys)
            keyNames.append(key->getName());

        // Initialize the stage rect
        TextRectItem stageRect(keyNames, QFont("Mono", 12));

        // Get height
        qreal height = stageRect.getHeight();
        if(height > tallest)
            tallest = height;

        // Set X position
        if(stages.length() != 0)
            stageRect.setPos(stages.last().getPos() + QPointF(stages.last().getWidth()+gates.last().getWidth(), 0));

        // Add title
        stageRect.addLineAt(0, "Stage " + QString::number(stage->getID()), QFont("Mono", 12, QFont::Bold));

        // Append to stage list
        stages.append(stageRect);

        // If the stage has a gate, initialize the gate
        if(stage->getExitGate())
        {
            TextRectItem gate;
            gate.addLine(stage->getExitGate()->getName(), QFont("Mono", 12, QFont::Bold));
            gate.setPos(stages.last().getPos() + QPointF(stages.last().getWidth(), (stages.last().getHeight()/2)-(gate.getHeight()/2)));
            gate.setBrush(QBrush(Qt::lightGray));
            gates.append(gate);
        }
    }

    for(TextRectItem stage : stages)
    {
        stage.setPos(QPointF(stage.getPos().x(), (tallest/2)-(stage.getHeight()/2)));
        addItem(stage.getRect());
    }

    for(TextRectItem gate : gates)
    {
        gate.setPos(QPointF(gate.getPos().x(), (tallest/2)-(gate.getHeight()/2)));
        addItem(gate.getRect());
    }
}


void MissionStructureScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    event->accept();
}

void MissionStructureScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    event->accept();
}

void MissionStructureScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
    // Change colour of text-rects to show where the mouse is....
    for(TextRectItem stage : stages)
    {
        if(stage.getRect()->boundingRect().contains(event->scenePos()))
            stage.setBrush(QBrush(Qt::red));
        else
            stage.setBrush(QBrush(Qt::white));
    }

    event->accept();
}

void MissionStructureScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    // Find the current stage (if any)
    for(TextRectItem stage : stages)
    {
        if(stage.getRect()->boundingRect().contains(event->scenePos()))
            stage.setBrush(QBrush(Qt::white)); // Clear the colour highlight
    }

    event->accept();
}
