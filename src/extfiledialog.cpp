#include "extfiledialog.h"
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include "settings.h"

ExtFileDialog::ExtFileDialog(QWidget *parent) :
    QFileDialog(parent)
{
    setOption(QFileDialog::DontUseNativeDialog);
    setDefaultSuffix("sql");
    QGridLayout* mainLayout = qobject_cast<QGridLayout*>(layout());

    if (!mainLayout)
        return;
    else
    {
        QHBoxLayout *hbl = new QHBoxLayout();
        _encodingCombo = new QComboBox(this);
        hbl->addStretch();
        hbl->addWidget(new QLabel(tr("Encoding")));
        hbl->addWidget(_encodingCombo);
        int numRows = mainLayout->rowCount();
        mainLayout->addLayout(hbl, numRows,0,1,-1);
    }
}

QString ExtFileDialog::encoding()
{
    return _encodingCombo->currentText();
}

void ExtFileDialog::setEncoding(const QString &encoding)
{
    int ind = _encodingCombo->findData(encoding, Qt::DisplayRole);
    if (ind == -1)
    {
        if (encoding.isEmpty())
        {
            if (!_encodingCombo->count())
                _encodingCombo->addItem("UTF-8");
            ind = 0;
        }
        else
        {
            _encodingCombo->addItem(encoding);
            ind = _encodingCombo->count() - 1;
        }
    }
    _encodingCombo->setCurrentIndex(ind);
}

void ExtFileDialog::fillEncodings()
{
    _encodingCombo->clear();
    _encodingCombo->addItems(
                SqtSettings::value("encodings").toString().
                split(',',
                  #if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
                      Qt::SkipEmptyParts
                  #else
                      QString::SkipEmptyParts
                  #endif
                      ));
    if (!_encodingCombo->count())
        _encodingCombo->setCurrentIndex(0);
}
