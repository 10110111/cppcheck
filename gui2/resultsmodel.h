#ifndef RESULTSMODEL_H
#define RESULTSMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QString>

class ResultsModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit ResultsModel(QObject *parent = 0);

    class Node {
    public:
        Node() {
            parent = 0;
        }

        Node(Node *parent, QString filename, QString line, QString severity, QString text, QString id) {
            this->parent   = parent;
            if (parent) {
                this->parent->allchildren.append(this);
                this->parent->shownchildren.append(this);
            }
            this->fullfilename = filename;
            this->filename = filename;
            this->line     = line;
            this->severity = severity;
            this->text     = text;
            this->id       = id;
        }

        ~Node() {
            qDeleteAll(allchildren);
        }

        Node *parent;
        QString fullfilename;
        QString filename;
        QString line;
        QString severity;
        QString text;
        QString id;
        QList<Node*> allchildren;
        QList<Node*> shownchildren;
    };

    void clear();
    void addresult(const QString &path, const QString &errmsg);

    void hideId(int row);
    void hideAllOtherId(int row);
    void showAll();

    bool load(const QString &fileName);
    bool save(const QString &fileName, const QString &projectName) const;

    Node getNodeFromIndex(const QModelIndex &index) const {
        Node *n = nodeFromIndex(index);
        if (n)
            return *n;
        return Node();
    }

private:
    void setRootNode(Node *node);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    static bool parseErrorMessage(const QString &errmsg, QString *file, QString *line, QString *severity, QString *text, QString *id);
    Node *nodeFromIndex(const QModelIndex &index) const;
    Node *rootNode;
};
#endif // RESULTSMODEL_H
