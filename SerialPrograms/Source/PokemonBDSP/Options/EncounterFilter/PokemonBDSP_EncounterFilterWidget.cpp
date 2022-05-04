/*  Encounter Filter
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include <QVBoxLayout>
#include <QLabel>
#include "Common/Cpp/Exceptions.h"
#include "Common/Qt/NoWheelComboBox.h"
#include "PokemonBDSP_EncounterFilterEnums.h"
#include "PokemonBDSP_EncounterFilterWidget.h"

namespace PokemonAutomation{
namespace NintendoSwitch{
namespace PokemonBDSP{



EncounterFilterWidget::EncounterFilterWidget(QWidget& parent, EncounterFilterOption& value)
    : QWidget(&parent)
    , ConfigWidget(value, *this)
    , m_value(value)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
//    QLabel* text = new QLabel(value.label(), this);
//    layout->addWidget(text);

    {
//        QWidget* widget = new QWidget(this);

        QHBoxLayout* hbox = new QHBoxLayout();
        layout->addLayout(hbox);
        hbox->addWidget(new QLabel("<b>Stop on:</b>"));

        m_shininess = new NoWheelComboBox(this);
        hbox->addWidget(m_shininess);
        for (const QString& item : ShinyFilter_NAMES){
            m_shininess->addItem(item);
        }
        ShinyFilter current = m_value.m_shiny_filter_current.load(std::memory_order_acquire);
        for (int c = 0; c < m_shininess->count(); c++){
            if (m_shininess->itemText(c) == ShinyFilter_NAMES[(int)current]){
                m_shininess->setCurrentIndex(c);
                break;
            }
        }
        connect(
            m_shininess, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index){
                if (index < 0){
                    return;
                }

                QString text = m_shininess->itemText(index);
                auto iter = ShinyFilter_MAP.find(text);
                if (iter == ShinyFilter_MAP.end()){
                    throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Invalid option: " + text.toStdString());
                }
                m_value.m_shiny_filter_current.store(iter->second, std::memory_order_release);
            }
        );
    }

    if (m_value.m_enable_overrides){
        layout->addSpacing(5);
        m_table = value.m_table.make_ui(*this);
        layout->addWidget(&m_table->widget());
    }
}
void EncounterFilterWidget::restore_defaults(){
    m_value.restore_defaults();
    update_ui();
}
void EncounterFilterWidget::update_ui(){
    ShinyFilter current = m_value.m_shiny_filter_current.load(std::memory_order_acquire);
    for (int c = 0; c < m_shininess->count(); c++){
        if (m_shininess->itemText(c) == ShinyFilter_NAMES[(int)current]){
            m_shininess->setCurrentIndex(c);
            break;
        }
    }
    if (m_table){
        m_table->update_ui();
    }
}



}
}
}
