#include <iostream>
#include "PInvoke.h"
#include "flatbuffers/flatbuffers.h"
#include "Monster_generated.h"
using namespace MyGame::Sample;

int Add(int a, int b)
{
    return a + b;
}

void CreateTestMonster(CopyMonsterCallback cb)
{
    // Create a `FlatBufferBuilder`, which will be used to create our
    // monsters' FlatBuffers.
    flatbuffers::FlatBufferBuilder builder(1024);

    auto weapon_one_name = builder.CreateString("大師之劍");
    short weapon_one_damage = 3;

    auto weapon_two_name = builder.CreateString("戰神之斧");
    short weapon_two_damage = 5;

    // Use the `CreateWeapon` shortcut to create Weapons with all the fields set.
    auto sword = CreateWeapon(builder, weapon_one_name, weapon_one_damage);
    auto axe = CreateWeapon(builder, weapon_two_name, weapon_two_damage);

    // Serialize a name for our monster, called "Orc".
    auto name = builder.CreateString("薩爾達公主(黑化)");

    // Create a `vector` representing the inventory of the Orc. Each number
    // could correspond to an item that can be claimed after he is slain.
    unsigned char treasure[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto inventory = builder.CreateVector(treasure, 10);

    // Place the weapons into a `std::vector`, then convert that into a FlatBuffer `vector`.
    std::vector<flatbuffers::Offset<Weapon>> weapons_vector;
    weapons_vector.push_back(sword);
    weapons_vector.push_back(axe);
    auto weapons = builder.CreateVector(weapons_vector);

    Vec3 points[] = { Vec3(1.0f, 2.0f, 3.0f), Vec3(4.0f, 5.0f, 6.0f) };
    auto path = builder.CreateVectorOfStructs(points, 2);

    // Create the position struct
    auto position = Vec3(1.0f, 2.0f, 3.0f);
    
    // Set his hit points to 300 and his mana to 150.
    int hp = 300;
    int mana = 150;
    
    // Finally, create the monster using the `CreateMonster` helper function
    // to set all fields.
    auto orc = CreateMonster(builder, &position, mana, hp, name, inventory,
                            Color_Red, weapons, Equipment_Weapon, axe.Union(),
                            path);

    builder.Finish(orc);

    // This must be called after `Finish()`.
    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize(); // Returns the size of the buffer that
                                // `GetBufferPointer()` points to.
    cb(buf, size);
}

void PrintMonster(uint8_t* buf)
{
    auto mon = GetMonster(buf);
    std::cout << mon->name()->string_view() << " " << mon->name()->string_view().size() << std::endl;
    
    if (mon->weapons())
    {
        for (const auto& weapon : *mon->weapons())
        {
            std::cout << weapon->name()->string_view() << " " << weapon->name()->string_view().size() << std::endl;
        }
    }

    if (mon->inventory())
    {
        for (const auto& it : *mon->inventory())
        {
            std::cout << (int) it << " ";
        }
        std::cout << std::endl;
    }
}