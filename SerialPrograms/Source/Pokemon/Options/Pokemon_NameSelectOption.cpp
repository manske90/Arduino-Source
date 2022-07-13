/*  Pokemon Name Select
 *
 *  From: https://github.com/PokemonAutomation/Arduino-Source
 *
 */

#include "Common/Cpp/Exceptions.h"
#include "Common/Cpp/Json/JsonValue.h"
#include "Common/Cpp/Json/JsonArray.h"
#include "CommonFramework/Globals.h"
#include "CommonFramework/Logging/LoggerQt.h"
#include "Pokemon/Resources/Pokemon_PokemonNames.h"
#include "PokemonSwSh/Resources/PokemonSwSh_PokemonSprites.h"
#include "Pokemon_NameSelectOption.h"

namespace PokemonAutomation{
namespace Pokemon{



PokemonNameSelectData::PokemonNameSelectData(const std::vector<std::string>& slugs){
    for (const std::string& slug : slugs){
        if (slug.size() <= 0){
            throw InternalProgramError(nullptr, PA_CURRENT_FUNCTION, "Expected non-empty string for Pokemon slug.");
        }

        using namespace NintendoSwitch::PokemonSwSh;
        const PokemonNames& data = get_pokemon_name(slug);
        const SpriteDatabase::Sprite* sprite = ALL_POKEMON_SPRITES().get_nothrow(slug);
        if (sprite == nullptr){
            m_list.emplace_back(data.display_name(), QIcon());
            global_logger_tagged().log("Missing sprite for: " + slug, COLOR_RED);
        }else{
            m_list.emplace_back(data.display_name(), sprite->icon);
        }
    }
}
PokemonNameSelectData::PokemonNameSelectData(const std::string& json_file_slugs){
    std::string path = RESOURCE_PATH().toStdString() + json_file_slugs;
    JsonValue json_slugs = load_json_file(path);
    JsonArray& slugs = json_slugs.get_array_throw(path);

    for (auto& item : slugs){
        std::string& slug = item.get_string_throw(path);

        using namespace NintendoSwitch::PokemonSwSh;
        const PokemonNames& data = get_pokemon_name(slug);
        const SpriteDatabase::Sprite* sprite = ALL_POKEMON_SPRITES().get_nothrow(slug);
        if (sprite == nullptr){
            m_list.emplace_back(data.display_name(), QIcon());
            global_logger_tagged().log("Missing sprite for: " + slug, COLOR_RED);
        }else{
            m_list.emplace_back(data.display_name(), sprite->icon);
        }
    }
}



PokemonNameSelect::PokemonNameSelect(
    std::string label,
    const std::vector<std::string>& slugs,
    const std::string& default_slug
)
    : PokemonNameSelectData(slugs)
    , StringSelectOption(
        std::move(label),
        cases(),
        default_slug.empty() ? "" :get_pokemon_name(default_slug).display_name()
    )
{}
PokemonNameSelect::PokemonNameSelect(
    std::string label,
    const std::string& json_file_slugs,
    const std::string& default_slug
)
    : PokemonNameSelectData(json_file_slugs)
    , StringSelectOption(
        std::move(label),
        cases(),
        default_slug.empty() ? "" :get_pokemon_name(default_slug).display_name()
    )
{}

const std::string& PokemonNameSelect::slug() const{
    const std::string& display = (const std::string&)*this;
    return parse_pokemon_name(display);
}




}
}
