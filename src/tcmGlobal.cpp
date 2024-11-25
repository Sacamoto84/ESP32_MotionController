#include "tmcGlobal.h"
#include "global.h"

uint8_t buf20[20] = {0};



/*
Формирование репорти типа 0
[0] - 'R'
[1] - 0 номер репорта
[2] -------------
 бит  [0] - двигатель включен выключен 
      [1] - shop/cicle 0-chop 1-cycle

[3..4] ток двигателя u16
[5] микрошаг 0->0 1->2 2->4 3->8 4->16 5->32 6->64 7->128 8->256 
[6] 
*/
void report0(){
    memset(buf20, 0, sizeof(buf20));

    buf20[0] = 'R';
    buf20[2] = tmcDriverEnable.get() | (tmcDriverChop.get()<<1);
    

}