using System.Runtime.InteropServices;
using MyGame.Sample;
using FlatBuffers;

public delegate void CopyMonsterCallback(IntPtr data, int dataLen);

public static class Program
{
    public static void Main()
    {
        SendToCpp();
        ReceiveFromCpp();
    }

    [DllImport("../build/cpp/libPInvoke.so")]
    public static extern void CreateTestMonster(CopyMonsterCallback cb);

    [DllImport("../build/cpp/libPInvoke.so")]
    public static extern void PrintMonster(byte[] buf);

    public static void SendToCpp()
    {
        var builder = new FlatBufferBuilder(1024);
        var weaponOneName = builder.CreateString("百穴菜刀");
        var weaponOneDamage = 3;
        
        var weaponTwoName = builder.CreateString("七星劍");
        var weaponTwoDamage = 5;

        // Use the `CreateWeapon()` helper function to create the weapons, since we set every field.
        var sword = Weapon.CreateWeapon(builder, weaponOneName, (short)weaponOneDamage);
        var axe = Weapon.CreateWeapon(builder, weaponTwoName, (short)weaponTwoDamage);

        // Serialize a name for our monster, called "Orc".
        var name = builder.CreateString("小當家");

        var inv = Monster.CreateInventoryVector(builder, Enumerable.Range(0, 10).Select(i => (byte)i).ToArray());
        var weaps = new Offset<Weapon>[2];
        weaps[0] = sword;
        weaps[1] = axe;
        
        // Pass the `weaps` array into the `CreateWeaponsVector()` method to create a FlatBuffer vector.
        var weapons = Monster.CreateWeaponsVector(builder, weaps);

        Monster.StartPathVector(builder, 2);
        Vec3.CreateVec3(builder, 1.0f, 2.0f, 3.0f);
        Vec3.CreateVec3(builder, 4.0f, 5.0f, 6.0f);
        var path = builder.EndVector();

        // Create our monster using `StartMonster()` and `EndMonster()`.
        Monster.StartMonster(builder);
        Monster.AddPos(builder, Vec3.CreateVec3(builder, 1.0f, 2.0f, 3.0f));
        Monster.AddHp(builder, (short)300);
        Monster.AddName(builder, name);
        Monster.AddInventory(builder, inv);
        Monster.AddColor(builder, Color.Red);
        Monster.AddWeapons(builder, weapons);
        Monster.AddEquippedType(builder, Equipment.Weapon);
        Monster.AddEquipped(builder, axe.Value); // Axe
        Monster.AddPath(builder, path);
        var orc = Monster.EndMonster(builder);

        // Call `Finish()` to instruct the builder that this monster is complete.
        builder.Finish(orc.Value); // You could also call `Monster.FinishMonsterBuffer(builder, orc);`.
    
        // Alternatively this copies the above data out of the ByteBuffer for you:
        byte[] buf = builder.SizedByteArray();
        PrintMonster(buf);
    }

    public static void ReceiveFromCpp()
    {
        Monster mon = default(Monster);
        CopyMonsterCallback foo = (IntPtr dataPtr, int dataLen) =>
        {
            var data = new byte[dataLen];
            Marshal.Copy(dataPtr, data, 0, dataLen);
            var bb = new ByteBuffer(data);
            mon = Monster.GetRootAsMonster(bb);
        };
        CreateTestMonster(foo);
        Console.WriteLine(mon.Name);
        for (var i = 0; i < mon.WeaponsLength; ++i)
        {
            Console.WriteLine(mon.Weapons(i)?.Name);
        }

        for (var i = 0; i < mon.InventoryLength; ++i)
        {
            Console.Write($"{mon.Inventory(i).ToString()} ");
        }
        Console.WriteLine();
    }
}