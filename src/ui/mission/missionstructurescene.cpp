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

    // Construct lists of stages and gates
    for(Stage* stage : missionStages)
    {
        QList<QString> keyNames; // Names of keys in this stage
        QList<Key*> keys = stage->getKeys(); // List of keys in this stage
        for(Key* key : keys)
            keyNames.append(key->getName());

        // Initialize the stage rect
        TextRectItem stageRect(keyNames, QFont("Mono", 12));

        // Set position
        if(stages.length() != 0)
        {
            stageRect.setPos(stages.last().getPos() + QPointF(stages.last().getWidth()+gates.last().getWidth(), 0));
        }

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
            gates.append(gate);
        }
    }

    // Add items to scene
    for(TextRectItem stage : stages)
        addItem(stage.getRect());

    for(TextRectItem gate : gates)
        addItem(gate.getRect());
}
