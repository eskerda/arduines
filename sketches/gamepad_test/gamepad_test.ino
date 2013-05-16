/* Based on Arduino USB Joystick HID demo
 * Author: Darran Hunt
 * 
 * https://github.com/harlequin-tech/arduino-usb
 */

int PADS[][2] = {
    {0,0},
    {0,1},
    {1,1},
    {1,0},
    {1,-1},
    {0,-1},
    {-1,-1},
    {-1,0},
    {-1,1},
    {0,1},
    {0,0}
};

int AXIS_VAL = 1;
int N_PADS = sizeof(PADS) / (sizeof(int)*2);
struct {
    int8_t x;
    int8_t y;
    uint8_t buttons;
} joyReport;

void setup();
void loop();

void setup() 
{
    Serial.begin(115200);
    delay(5000);
}

void loop() 
{
    joyReport.buttons = 0;
    joyReport.x = 0;
    joyReport.y = 0;
    
    /* Make the pad do a clockwise barrel roll */
    for (int i = 0; i < N_PADS; i++){
        joyReport.x = PADS[i][0] * AXIS_VAL;
        joyReport.y = PADS[i][1] * - AXIS_VAL;
        Serial.write((uint8_t *)&joyReport, 4);
        delay(250);
    }
    
    /* Press every combination ever of buttons 
     * That is.. button per bit, everything from
     * 0x00 to 0x0F
     */
    for (int i = 0; i <= 0x0F; i++){
        joyReport.buttons = i;
        Serial.write((uint8_t *)&joyReport, 4);
        delay(250);
    }     
    delay(1000);
}
