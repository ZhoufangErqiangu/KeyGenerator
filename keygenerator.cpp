#include "keygenerator.h"
#include "ui_keygenerator.h"

KeyGenerator::KeyGenerator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KeyGenerator)
{
    ui->setupUi(this);
    Init();
}

KeyGenerator::~KeyGenerator()
{
    delete ui;
}

QString KeyGenerator::GenerateKey(bool bUpperCheck, bool bLowerCheck, bool bNumCheck, bool bSpecialCheck, int iKeyLength)
{
    int i=0, iKeyCharASCValue;
    QString sKeyString="", sKeyChar;
    QList<int> ilKeyChars;

    ilKeyChars.clear();
    if (bUpperCheck==true) //add A..Z A->65 ... Z->90
    {
        for (i=0;i<26;i++)
        {
            ilKeyChars<<i+65;
        }
    }
    if (bLowerCheck==true) //add a..z a->97 ... z->122
    {
        for (i=0;i<26;i++)
        {
            ilKeyChars<<i+97;
        }
    }
    if (bNumCheck==true) //add 0..9 0->48 ... 9->57 !->33 @->64 #->35 $->36 %->37 ^->94 &->38 *->42

    {
        for (i=0;i<10;i++)
        {
            ilKeyChars<<i+48;
        }
    }
    if (bSpecialCheck==true) //add !@#$%^&*
    {
        ilKeyChars<<33<<64<<35<<36<<37<<94<<38<<42;
    }

    iKeyLength=ui->KeyLengthSpinBox->value();
    if (ilKeyChars.isEmpty()==false and iKeyLength>0)
    {
        for (i=0;i<iKeyLength;i++)
        {
            iKeyCharASCValue=QRandomGenerator::global()->bounded(0,ilKeyChars.length());
            sKeyChar=char(ilKeyChars.at(iKeyCharASCValue));
            sKeyString+=sKeyChar;
        }
        ui->statusBar->showMessage("Generated OK.");
    }
    else
    {
        ui->statusBar->showMessage("Please confirm check box and length.");
    }

    return (sKeyString);
}

void KeyGenerator::ReadConfig()
{
    QString sConfigFilePath;
    QFile *fConfigFile;
    Qt::CheckState bUpperCheck, bLowerCheck, bNumCheck, bSpecialCheck;
    int iKeyLength;

    sConfigFilePath=QDir::currentPath()+"/config.ini";
    fConfigFile = new QFile(sConfigFilePath);
    QSettings *stConfigFile;

    if (fConfigFile->exists()==true)
    {
        stConfigFile=new QSettings(sConfigFilePath,QSettings::IniFormat);
        ui->statusBar->showMessage("Read config file OK.");
        bUpperCheck=   static_cast<Qt::CheckState>(stConfigFile->value("UI/UpperCheck").toInt());
        bLowerCheck=   static_cast<Qt::CheckState>(stConfigFile->value("UI/LowerCheck").toInt());
        bNumCheck=     static_cast<Qt::CheckState>(stConfigFile->value("UI/NumCheck").toInt());
        bSpecialCheck= static_cast<Qt::CheckState>(stConfigFile->value("UI/SpecialCheck").toInt());
        iKeyLength=                                stConfigFile->value("UI/KeyLength").toInt();
    }
    else
    {
        ui->statusBar->showMessage("Config file not exists.");
        bUpperCheck=Qt::Checked;
        bLowerCheck=Qt::Checked;
        bNumCheck=Qt::Checked;
        bSpecialCheck=Qt::Unchecked;
        iKeyLength=16;
    }
    ui->UpperCharCheckBox->setCheckState(bUpperCheck);
    ui->LowerCharCheckBox->setCheckState(bLowerCheck);
    ui->NumberCheckBox->setCheckState(bNumCheck);
    ui->SpecialCharChechBox->setCheckState(bSpecialCheck);
    ui->KeyLengthSpinBox->setValue(iKeyLength);
}

void KeyGenerator::WriteConfig()
{
    QString sConfigFilePath;
    //QFile *fConfigFile;
    Qt::CheckState bUpperCheck, bLowerCheck, bNumCheck, bSpecialCheck;
    int iKeyLength;

    sConfigFilePath=QDir::currentPath()+"/config.ini";
    //fConfigFile = new QFile(sConfigFilePath);
    QSettings *stConfigFile;

    bUpperCheck=ui->UpperCharCheckBox->checkState();
    bLowerCheck=ui->LowerCharCheckBox->checkState();
    bNumCheck=ui->NumberCheckBox->checkState();
    bSpecialCheck=ui->SpecialCharChechBox->checkState();
    iKeyLength=ui->KeyLengthSpinBox->value();

    stConfigFile=new QSettings(sConfigFilePath,QSettings::IniFormat);
    stConfigFile->setValue("UI/UpperCheck",bUpperCheck);
    stConfigFile->setValue("UI/LowerCheck",bLowerCheck);
    stConfigFile->setValue("UI/NumCheck",bNumCheck);
    stConfigFile->setValue("UI/SpecialCheck",bSpecialCheck);
    stConfigFile->setValue("UI/KeyLength",iKeyLength);

    ui->statusBar->showMessage("Write config file OK.");
}

void KeyGenerator::Init()
{
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(End()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(About()));
    connect(ui->actionGenerate,SIGNAL(triggered()),this,SLOT(SlotGenerate()));
    connect(ui->GenerateButton,SIGNAL(clicked()),this,SLOT(SlotGenerate()));
    ReadConfig();
}

void KeyGenerator::About()
{
    QMessageBox::aboutQt(this);
}

void KeyGenerator::SlotGenerate()
{
    ui->OutputBrowser->setText(GenerateKey(ui->UpperCharCheckBox->checkState(),
                                           ui->LowerCharCheckBox->checkState(),
                                           ui->NumberCheckBox->checkState(),
                                           ui->SpecialCharChechBox->checkState(),
                                           ui->KeyLengthSpinBox->value()));
}

void KeyGenerator::closeEvent(QCloseEvent *event)
{
    WriteConfig();
    event->accept();
}
