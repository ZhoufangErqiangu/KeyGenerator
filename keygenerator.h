#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QCloseEvent>

namespace Ui {
class KeyGenerator;
}

class KeyGenerator : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);

public:
    explicit KeyGenerator(QWidget *parent = nullptr);
    ~KeyGenerator();

private slots:
    void Init();
    void About();
    void SlotGenerate();

private:
    Ui::KeyGenerator *ui;    

    int iChars[256];

    QString GenerateKey(bool bUpperCheck, bool bLowerCheck, bool bNumCheck, bool bSpecialCheck, int iKeyLength);
    void ReadConfig();
    void WriteConfig();
};

#endif // KEYGENERATOR_H
