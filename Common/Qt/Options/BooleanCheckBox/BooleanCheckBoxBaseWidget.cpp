/*  Boolean Check Box Option
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 *      This option is thread-safe.
 *
 */

#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "BooleanCheckBoxBaseWidget.h"

namespace PokemonAutomation{



BooleanCheckBoxBaseWidget::BooleanCheckBoxBaseWidget(QWidget& parent, BooleanCheckBoxBaseOption& value)
    : QWidget(&parent)
    , m_value(value)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    QLabel* text = new QLabel(QString::fromStdString(m_value.label()), this);
    text->setWordWrap(true);
    layout->addWidget(text, 3);
    m_box = new QCheckBox(this);
    m_box->setChecked(m_value);
    layout->addWidget(m_box, 1);
    connect(
        m_box, &QCheckBox::stateChanged,
        this, [=](int){
            m_value.set(m_box->isChecked());
//            cout << "m_value.m_current = " << m_value.m_current << endl;
//            cout << "&m_value.m_current = " << &m_value.m_current << endl;
        }
    );
}
void BooleanCheckBoxBaseWidget::restore_defaults(){
    m_value.restore_defaults();
    update_ui();
}
void BooleanCheckBoxBaseWidget::update_ui(){
    m_box->setChecked(m_value);
}




}
