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
    updateLinePositions();
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

QGraphicsRectItem* TextRectItem::getRect() const
{
    return rect;
}

QList<QGraphicsSimpleTextItem*> TextRectItem::getGraphicLines() const
{
    return graphicLines;
}

QGraphicsSimpleTextItem* TextRectItem::getGraphicLineAt(int index)
{
    return graphicLines.at(index);
}

StageRect::StageRect(Stage* stage)
{
    this->stage = stage;

    QList<Key*> keys = stage->getKeys();
    for(Key* key : keys)
        addLine(key->getName() + "\n(" + KEY_TYPE_STRINGS[key->getKeyType()] + ")", QFont("Mono", 12));
}

void StageRect::update()
{
    clear();
    QList<Key*> keys = stage->getKeys();
    for(Key* key : keys)
        addLine(key->getName() + "\n(" + KEY_TYPE_STRINGS[key->getKeyType()] + ")", QFont("Mono", 12));
}

StageRect::~StageRect()
{

}

GateRect::GateRect(Gate* gate)
{
    this->gate = gate;

    addLine(gate->getName(), QFont("Mono", 12, QFont::Weight::Bold));
    QList<Key*> keys = gate->getKeys();
    for(Key* key : keys)
        addLine(key->getName(), QFont("Mono", 12));
}

GateRect::~GateRect()
{

}

void GateRect::update()
{
    clear();
    QList<Key*> keys = gate->getKeys();
    for(Key* key : keys)
        addLine(key->getName() + "\n(" + KEY_TYPE_STRINGS[key->getKeyType()] + ")", QFont("Mono", 12));
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
    stages = QList<StageRect>();
    gates = QList<TextRectItem>();

    QList<Stage*> missionStages = mission->getStages();

    qreal tallest = 0.f;

    // Construct lists of stages and gates
    for(Stage* stage : missionStages)
    {
        // Initialize the stage rect
        StageRect stageRect(stage);

        // Get height
        qreal height = stageRect.getHeight();
        if(height > tallest)
            tallest = height;

        // Set X position
        if(stages.length() != 0)
            stageRect.setPos(stages.last().getPos() + QPointF(stages.last().getWidth()+gates.last().getWidth(), 0));

        // Append to stage list
        stages.append(stageRect);

        // If the stage has a gate, initialize the gate
        if(stage->getExitGate())
        {
            GateRect gate(stage->getExitGate());
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
        QRectF bounds = stage.getRect()->boundingRect();
        bounds.translate(stage.getRect()->pos().x(), stage.getRect()->pos().y());

        if(bounds.contains(event->scenePos()))
            stage.setBrush(QBrush(Qt::red));
        else
            stage.setBrush(QBrush(Qt::white));
    }

    event->accept();
}

void MissionStructureScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    // Find the current stage (if any)
    for(int i = 0; i < stages.size(); i++)
    {
        stages.at(i).setBrush(QBrush(Qt::white)); // Clear the colour highlight

        // Check if we have dropped they key into a stage
        QRectF bounds = stages.at(i).getRect()->boundingRect();
        bounds.translate(stages.at(i).getRect()->pos().x(), stages.at(i).getRect()->pos().y());
        if(bounds.contains(event->scenePos()))
        {
            const QMimeData* data = event->mimeData();
            if(data->hasText())
            {
                QString keyName = data->text();

                Stage* stage = stages.at(i).getStage();
                Key* key = mission->getKeyEvent(keyName);

                // Only add the key if it does not invalidate the mission
                if(mission->validateAddition(stage, key))
                {
                    if(!mission->doesKeyExist(key)) // Also ignore if the key already exists
                        stage->addKey(key);
                    else
                        QMessageBox::warning(static_cast<QWidget*>(parent()), "Error", "This key already exists in the structure. BrushRemove it before adding it elsewhere.",
                                                     QMessageBox::Ok);

                }
                else
                    QMessageBox::warning(static_cast<QWidget*>(parent()), "Error", "This would create an invalid and impossible mission with the current structure.",
                                                 QMessageBox::Ok);

                missionUpdated();
            }
        }
    }

    event->accept();
}

void MissionStructureScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    for(TextRectItem stage : stages)
    {
        QList<QGraphicsSimpleTextItem*> lines = stage.getGraphicLines();
        for(QGraphicsSimpleTextItem* line : lines)
        {
            QRectF bounds = line->boundingRect();
            bounds.translate(line->pos().x(), line->pos().y());
            bounds.translate(stage.getRect()->pos().x(), stage.getRect()->pos().y());
            if(bounds.contains(event->scenePos()))
            {
                line->setBrush(QBrush(Qt::red));
            }
            else
            {
                line->setBrush(QBrush(Qt::black));
            }
        }
    }

    for(TextRectItem gate : gates)
    {
        QRectF bounds = gate.getRect()->boundingRect();
        bounds.translate(gate.getRect()->pos().x(), gate.getRect()->pos().y());
        if(bounds.contains(event->scenePos()))
            gate.setBrush(QBrush(Qt::red));
        else
            gate.setBrush(QBrush(Qt::gray));
    }
}

void MissionStructureScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
