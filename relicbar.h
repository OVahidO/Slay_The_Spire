#ifndef RELICBAR_H
#define RELICBAR_H

#include <QWidget>
#include <QLabel>

class Relic;

namespace Ui {
class RelicBar;
}

class RelicBar : public QWidget
{
    Q_OBJECT

public:
    explicit RelicBar(QWidget *parent = nullptr);

public slots:
    void addRelic(Relic *relic);
    void clearRelics();

private:
    Ui::RelicBar *ui;
    QLabel *m_descriptionLabel;
};

#endif // RELICBAR_H
