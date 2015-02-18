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

StageRect::StageRect(QString title, Stage* stage)
{
    this->stage = stage;

    addLine(title, QFont("Mono", 14, QFont::Weight::Bold));

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
    highlightedGate = nullptr;
    highlightedKey = nullptr;
    highlightedStage = nullptr;

    keyContextMenu.addAction("Toggle Lock In", this, SLOT(toggleLockInKey()));
    keyContextMenu.addAction("Remove", this, SLOT(removeKey()));

    gateContextMenu.addAction("Toggle Lock In", this, SLOT(toggleLockInGate()));

    gateReplaceSubMenu = QSharedPointer<QMenu>(new QMenu());
    gateReplaceSubMenu.data()->setTitle("Replace With...");
    gateContextMenu.addMenu(gateReplaceSubMenu.data());
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
    gates = QList<GateRect>();

    QList<Stage*> missionStages = mission->getStages();

    qreal tallest = 0.f;

    // Construct lists of stages and gates
    for(Stage* stage : missionStages)
    {
        // Initialize the stage rect
        StageRect stageRect("STAGE " + QString::number(stages.count()), stage);

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
        gate.getRect()->setFlag(QGraphicsItem::ItemIsMovable, true);
        addItem(gate.getRect());
    }

    updateKeyHighlights();
    updateGateHighlights();
}

void MissionStructureScene::toggleLockInKey()
{
    if(highlightedStage != nullptr && highlightedKey != nullptr)
    {
        if(highlightedStage->getStage()->isKeyLocked(highlightedKey))
            highlightedStage->getStage()->unlockKey(highlightedKey);
        else
            highlightedStage->getStage()->lockKey(highlightedKey);

        updateKeyHighlights();
    }
}

void MissionStructureScene::removeKey()
{
    if(highlightedStage != nullptr && highlightedKey != nullptr)
    {
        highlightedStage->getStage()->removeKey(highlightedKey);
        missionUpdated();
    }
}

void MissionStructureScene::toggleLockInGate()
{
    if(highlightedGate != nullptr)
    {
        QList<Stage*> missionStages = mission->getStages();
        for(Stage* stage : missionStages)
        {
            if(stage->getExitGate() == highlightedGate->getGate())
            {
                if(stage->isGateLocked())
                    stage->setGateLocked(false);
                else
                    stage->setGateLocked(true);

                break;
            }
        }
    }

    updateGateHighlights();
}

void MissionStructureScene::replaceGate(QAction* action)
{
    QString name = action->data().toString();

    // Get the stage that CURRENTLY has the selected gate, and the stage we are replacing
    QList<Stage*> stageList = mission->getStages();

    Stage* currentOwner = nullptr;
    Stage* beingReplaced = nullptr;

    for(Stage* stage : stageList)
    {
        if(stage->getExitGate()->getName() == name)
            currentOwner = stage;
        if(stage->getExitGate()->getName() == highlightedGate->getGate()->getName())
            beingReplaced = stage;
    }

    // Unlock gates from the stages being swapped
    currentOwner->setGateLocked(false);
    beingReplaced->setGateLocked(false);

    // Swap the gates over
    Gate* moving = currentOwner->getExitGate();
    Gate* swapping = beingReplaced->getExitGate();

    currentOwner->setExitGate(swapping);
    beingReplaced->setExitGate(moving);

    QList<Key*> invalidatedKeys = mission->getInvalidKeys(); // Get any keys invalidated by this move

    // Validate the change
    if(invalidatedKeys.length() > 0)
    {
        QString invalidatedKeyNames;
        for(Key* key : invalidatedKeys)
            invalidatedKeyNames += key->getName() + ", ";
        invalidatedKeyNames.remove(invalidatedKeyNames.length()-2, 2);

        if(QMessageBox::warning(static_cast<QWidget*>(parent()), "Error", "This action would invalidate the mission. To make the swap valid, the keys: " +
                                                                            invalidatedKeyNames + " will need to be removed from the current structure. Do you wish to do this?",
                                QMessageBox::Button::Yes | QMessageBox::Button::No) == QMessageBox::Button::Yes)
        {
            // Find the stages that contain the keys, and then remove them
            for(Key* key : invalidatedKeys)
            {
                for(StageRect& stage : stages)
                {
                    if(stage.getStage()->getKeys().contains(key))
                        stage.getStage()->removeKey(key);
                }
            }

            beingReplaced->setGateLocked(true);
        }
        else // If the user doesn't wish to remove the keys, swap the gates back to their original positions
        {
            currentOwner->setExitGate(moving);
            beingReplaced->setExitGate(swapping);
        }
    }
    else
        beingReplaced->setGateLocked(true);

    missionUpdated();
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
                    {
                        stage->addKey(key);
                        stage->lockKey(key);
                    }
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

void MissionStructureScene::updateKeyHighlights()
{
    for(StageRect stage : stages)
    {
        QList<QGraphicsSimpleTextItem*> lines = stage.getGraphicLines();
        for(int i = 1; i < lines.length(); i++)
        {
            // Get the associated key
            QStringList list = lines[i]->text().split('\n');
            Key* key = stage.getStage()->getKey(list.at(0));

            if(key == highlightedKey)
                lines[i]->setBrush(QBrush(Qt::red));
            else if(stage.getStage()->isKeyLocked(key))
                lines[i]->setBrush(QBrush(Qt::blue));
            else
                lines[i]->setBrush(QBrush(Qt::black));
        }
    }
}

void MissionStructureScene::updateGateHighlights()
{
    for(StageRect& stage : stages)
    {
        // Find this stage's gate rect
        Gate* gatePtr = stage.getStage()->getExitGate();

        GateRect* rect;
        for(GateRect& gate : gates)
        {
            if(gatePtr->getName() == gate.getGate()->getName())
                rect = &gate;
        }

        if(stage.getStage()->isGateLocked())
            rect->setBrush(QBrush(Qt::blue));
        else
            rect->setBrush(QBrush(Qt::gray));

        if(highlightedGate != nullptr)
        {
            if(gatePtr == highlightedGate->getGate())
                rect->setBrush(QBrush(Qt::red));
        }
    }
}

void MissionStructureScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Update highlighted stage
    bool hitFound = false;
    for(StageRect& stage : stages)
    {
        if(rectContains(stage, event->scenePos()))
        {
            highlightedStage = &stage;
            hitFound = true;
            break;
        }
    }
    if(!hitFound)
        highlightedStage = nullptr;

    hitFound = false;
    if(highlightedStage != nullptr)
    {
        QList<QGraphicsSimpleTextItem*> lines = highlightedStage->getGraphicLines();
        for(int i = 1; i < lines.length(); i++)
        {
            // Get the associated key
            QStringList list = lines[i]->text().split('\n');
            Key* key = highlightedStage->getStage()->getKey(list.at(0));

            if(lineRectContains(highlightedStage->getRect()->pos(), lines[i], event->scenePos()))
            {
                // Update the highlighted key
                highlightedKey = key;
                hitFound = true;
            }
        }
    }

    if(!hitFound)
        highlightedKey = nullptr;

    // Update highlighted gate
    hitFound = false;
    for(GateRect& gate : gates)
    {
        if(rectContains(gate, event->scenePos()))
        {
            highlightedGate = &gate;
            hitFound = true;
            break;
        }
    }
    if(!hitFound)
        highlightedGate = nullptr;

    updateKeyHighlights();
    updateGateHighlights();
}

void MissionStructureScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    if(highlightedKey != nullptr)
        keyContextMenu.exec(event->screenPos());
    if(highlightedGate != nullptr)
    {
        gateReplaceSubMenu.data()->clear();

        // Get a list of gates to add to replacement menu
        QList<QString> list;
        for(GateRect& gate : gates)
        {
            if(gate.getGate() != highlightedGate->getGate())
                list.append(gate.getGate()->getName());
        }

        for(QString name : list)
        {
            QAction* action = gateReplaceSubMenu.data()->addAction(name);
            action->setData(name);
        }

        connect(gateReplaceSubMenu.data(), SIGNAL(triggered(QAction*)), this, SLOT(replaceGate(QAction*)), Qt::UniqueConnection);

        gateContextMenu.exec(event->screenPos());
    }
}

bool MissionStructureScene::rectContains(TextRectItem item, QPointF mousePos)
{
    QRectF bounds = item.getRect()->boundingRect();
    bounds.translate(item.getRect()->pos().x(), item.getRect()->pos().y());
    if(bounds.contains(mousePos))
        return true;

    return false;
}

bool MissionStructureScene::lineRectContains(QPointF parentPosition, QGraphicsSimpleTextItem* line, QPointF mousePos)
{
    QRectF bounds = line->boundingRect();
    bounds.translate(line->pos().x(), line->pos().y());
    bounds.translate(parentPosition.x(), parentPosition.y());
    if(bounds.contains(mousePos))
        return true;

    return false;
}
