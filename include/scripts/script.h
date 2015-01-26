#ifndef SCRIPT_H
#define SCRIPT_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <QMap>

/*!
 * \brief Represents a script file, use this to generate scripts for the game to use.
 */
class Script
{
public:
    class Item
    {
    public:
        Item(QString name) : name(name) { }

        QString getName() { return name; }

        virtual QString build() = 0;

    protected:
        QString name;
    };

    /**
     * @brief Represents an individual variable.
     */
    class Variable : public Item
    {
    public:
        Variable(QString name, QString value) : Item(name), value(value) { }

        virtual QString build() override final { return "local " + name + " = " + value; }

    private:
        QString value;
    };

    /**
     * @brief Represents an individual function.
     */
    class Function : public Item
    {
    public:
        Function(QString name) : Item(name) { }

        void addLine(QString line) { lines.append(line); }

        virtual QString build() override final;

    private:
        QStringList lines;
    };

    Script() { returnStatement = ""; }
    virtual ~Script() { }

    /*!
     * \brief Adds a priority variable to this script. The priority variable must already exist in the script before it is added, if it is not found
     *        this function returns false and does nothing. Priority variables are printed before all other variables, in the order they are added
     *        to this list.
     * \param name The name of the variable to add as priority.
     */
    bool addPriorityVariable(QString name);

    /*!
     * \brief Adds a priority function to this script. The function variable must already exist in the script before it is added, if it is not found
     *        this function returns false and does nothing. Priority functions are printed before all other functions, in the order they are added
     *        to this list.
     * \param name The name of the function to add as priority.
     */
    bool addPriorityFunction(QString name);

    Function* getFunction(QString name);

    /*!
     * \brief writeToFile Writes this script into the given file.
     * \param file The file to write the script into.
     */
    virtual void writeToFile(QFile& file);

protected:
    QList<QString> priorityVariables;
    QList<QString> priorityFunctions;

    QMap<QString,Function> functions;
    QMap<QString,Variable> variables;

    QString returnStatement;
};

#endif // SCRIPT_H
