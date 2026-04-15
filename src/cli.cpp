#include <Arduino.h>
#include <SimpleCLI.h>
#include "tmcGlobal.h"
#include "global.h"

extern void update();

SimpleCLI cli;

//Список команд
//Command cmdReset;  // Перезагрузка reset
//Command cmdEnable; // Включение отключение драйвера >>enable [on/off]
//Command cmdReadGCONF;

void printBits(uint32_t value)
{
    for (int i = 31; i >= 0; --i)
    { // 31-й бит - старший
        timber.i("%c", (value & (1U << i)) ? '1' : '0');
    }
    timber.i("\n");
}

void resetCallback(cmd *c)
{
    Command cmd(c);
    timber.i("Перезагрузка");
    esp_restart();
}

void enableCallback(cmd *c)
{
    Command cmd(c);
    Argument arg = cmd.getArgument(0);
    String argVal = arg.getValue();
    if (argVal == "on")
    {
        tmcStepperEnable.set(1);
        timber.i("Включить мотор");
        update();
    }
    else
    {
        tmcStepperEnable.set(0);
        timber.i("Выключить мотор");
        update();
    }
}

void errorCallback(cmd_error *errorPtr)
{
    CommandError e(errorPtr);
    timber.i("ERROR: %s", e.toString().c_str());
    if (e.hasCommand())
    {
        timber.println("Did you mean? " + e.getCommand().toString());
    }
    else
    {
        timber.println(cli.toString());
    }
}

void readGSONCallback(cmd *c)
{
    Command cmd(c);
    timber.i("Прочесть GCONF:");
    printBits(driver.GCONF());

    timber.i("Прочесть MSCNT:");
    printBits(driver.MSCNT());

    timber.i("Прочесть CHOPCONF:");
    printBits(driver.CHOPCONF());

    timber.i("Прочесть DRV_STATUS:");
    printBits(driver.DRV_STATUS());

}

void pingCallback(cmd *cmdPtr)
{
    Command cmd(cmdPtr);

    Argument argN = cmd.getArgument("num");
    String argVal = argN.getValue();
    int n = argVal.toInt();

    Argument argStr = cmd.getArgument("str");
    String strVal = argStr.getValue();

    Argument argC = cmd.getArgument("c");
    bool c = argC.isSet();

    if (c)
        strVal.toUpperCase();

    for (int i = 0; i < n; i++)
    {
        timber.i(strVal.c_str());
    }
}

void cliInit()
{

    // Сброс
    Command cmdReset = cli.addCommand("reset", resetCallback);
    Command cmdEnable = cli.addSingleArgCmd("enable", enableCallback);
    Command cmdReadGCONF = cli.addCommand("readGCONF", readGSONCallback);

    // cmdPing.addPositionalArgument("str", "pong");
    // cmdPing.addArgument("n/um/ber,anzahl", "1");
    // cmdPing.addFlagArgument("c");

    // cmdPong = cli.addBoundlessCommand("pong,hello", pongCallback);

    cli.setOnError(errorCallback);
}
