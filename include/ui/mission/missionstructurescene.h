#ifndef MISSIONSTRUCTURESCENE_H
#define MISSIONSTRUCTURESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>
#include <QBrush>
#include <QGraphicsSceneDragDropEvent>

#include "mission.h"

/*!
 * \brief Represents a rect containing the given text, sets rect size automatically.
 */
class TextRectItem
{
public:
    TextRectItem();

    virtual ~TextRectItem();

    /*!
     * \brief Creates a new text rect item and initializes it with some lines of text.
     * \param textLines The text you wish to display in the rect, split into individual lines.
     */
    TextRectItem(QList<QString> textLines, QFont font);

    /*!
     * \brief Adds a new line of text to the rect item.
     * \param line The line of text to add.
     */
    void addLine(QString line, QFont font);

    /*!
     * \brief Add a new line at the given index. If index is less than 0, adds to the start, and if its greater than the size of the line list, it is
     *        appended to the end.
     */
    void addLineAt(int index, QString line, QFont font);

    /*!
     * \brief Removes the given line from the rect. Returns false if the line was not found.
     * \param line The line to remove.
     */
    void removeLine(QString line);

    /*!
     * \brief Retrieve the graphics rect item.
     */
    QGraphicsRectItem* getRect();

    /*!
     * \brief Retrieve the list of graphic text items.
     */
    QList<QGraphicsSimpleTextItem*> getGraphicLines();

    /*!
     * \brief Retrieve a text graphic item at the given index. If the index is out-of-bounds, returns nullptr.
     */
    QGraphicsSimpleTextItem* getGraphicLineAt(int index);

    /*!
     * \brief Set the position of the rect.
     */
    void setPos(QPointF pos) { rect->setPos(pos); }

    QPointF getPos() { return rect->pos(); }

    qreal getWidth() { return rect->boundingRect().width(); }

    qreal getHeight() { return rect->boundingRect().height(); }

    void setBrush(QBrush brush) { rect->setBrush(brush); }

private:
    static const qreal PADDING;
    static const qreal LINE_SPACING;

    TextRectItem cpy(const TextRectItem& param); // Internal copy function, used by copy assign and constructor

    void updateRect(); // Updates the rectangle
    void updateLinePositions(); // Updates positions of all lines

    QBrush textBrush; /*!< Brush used to draw text. */
    QBrush rectBrush; /*!< Brush used to draw the rect. */
    QGraphicsRectItem* rect;
    QList<QString> lines;
    QList<QGraphicsSimpleTextItem*> graphicLines;
};

class MissionStructureScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MissionStructureScene(QObject *parent = 0);

    /*!
     * \brief Sets the mission to show in this structure scene.
     */
    void setMission(Mission* mission);
protected:
   void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
   void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
   void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
   void dropEvent (QGraphicsSceneDragDropEvent* event) override;
public slots:
    void missionUpdated();
private:
    Mission* mission;

    QList<TextRectItem> stages;
    QList<TextRectItem> gates;
};

#endif // MISSIONSTRUCTURESCENE_H
