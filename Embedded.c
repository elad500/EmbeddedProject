#include <xc.h>

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

void function1(char* stringUp, char* stringDown, int lenUp, int lenDown, int* startingPlace);
void busy(void);
void main (void)
{
    int j,i,k;
    int startingPlace = 0xb9;
    int Delay,fastDelay = 100000, slowDelay = 200000;
    char* stringUp="";
    char* stringDown = "";
    int lenUp,lenDown;
    char control[]={0x38,0x38,0x38,0xe,0x6,0x1};

    TRISBbits.TRISB14 = 0;//The corresponding TRIS bit must be set to 0:
    ANSELBbits.ANSB14 = 0;//The corresponding ANSEL bit should be set to 0:
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input 
    ANSELBbits.ANSB10 = 0; // RB10 (SW6) disabled analog
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog

    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISEbits.TRISE0 = 1; // RE0 (DB0) set as input (change 1 to 0 for

    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;

    TRISE &= 0xff00;
    TRISA &= 0xff00;
    int f = 0,t=100,b=100;

    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    ANSELEbits.ANSE7 = 0;

    for(i=0;i<6;i++)
    {
        PORTE=control[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        //for(j=0;j<32000;j++);
        busy();
    }

    while(1)
    {
        stringUp="";
        stringDown = "";
        lenUp = 0;
        lenDown = 0;
        if(PORTFbits.RF3 == 0){
            if(PORTBbits.RB11 == 0)
                PORTA = 0x00;
        }
        
        if(PORTBbits.RB9 == 0){
            
        
            //Switch number 5 (if defined as 0 - continue the iteration)
            if(PORTBbits.RB11 == 0)
            {
                //Switch number 4 define the speed of the action
                if(PORTDbits.RD14 == 0){
                    Delay = slowDelay;
                }
                else{
                    Delay = fastDelay;
                }
                //Switch number 6
                if(PORTBbits.RB10 == 1)
                {
                  PORTBbits.RB14^=1;
    
                for(;b<100+t;b++);
                    t+=100;
                    if(t>32000)
                        {
                            t=100;
                            b=10;
                        }
                    stringUp="mode 6:";
                    stringDown = "Beep Mode";
                    lenUp = 7;
                    lenDown = 9;
                }
                else{    
                //Switch number 0
                if(PORTFbits.RF3 == 1)
                {
                    //Switch number 3 == 0 --> do up counter
                    if(PORTDbits.RD15 == 0){
                        stringUp = "mode 0:";
                        if(Delay == slowDelay){
                            stringDown = "Counter Up Slow";
                            lenDown = 15;
                        }
                        else{
                            stringDown = "Counter Up Fast";
                            lenDown = 15;  
                        }
                        lenUp = 7;
                        PORTA++;
                    }
                    //Switch number 3 == 1 --> do down counter
                    else{
                        stringUp = "mode 0:";
                        if(Delay == slowDelay){
                            stringDown = "Counter Down Slow";
                            lenDown = 17;
                        }
                        else{
                            stringDown = "Counter Down Fast";
                            lenDown = 17;
                        }
                        lenUp = 7;
                        PORTA--;
                    }
                }
                else
                {
                    int numbers[8] = {1,2,4,8,16,32,64,128};

                    //Switch number 1
                    if(PORTFbits.RF5 == 1)
                    {
                        //Switch number 3 == 0 --> go Left counter
                        if(PORTDbits.RD15 == 0)
                        {
                            stringUp = "mode 1:";
                            if(Delay == slowDelay){
                                stringDown = "Counter Left Slow";
                                lenDown = 17;
                            }
                            else{
                                stringDown = "Counter Left Fast";
                                lenDown = 17;
                            }
                            lenUp = 7;
                            
                            PORTA = numbers[f];
                            if(f == 7)
                                f = 0;
                            else
                                f++;
                        }
                        //Switch number 3 == 1 --> go Right counter
                        else
                        {
                            stringUp = "mode 1:";
                            if(Delay == slowDelay){
                                stringDown = "Counter Right Slow";
                                lenDown = 18;
                            }
                            else{
                                stringDown = "Counter Right Fast";
                                lenDown = 18;
                            }
                            lenUp = 7;
                            
                            PORTA = numbers[f];
                            if(f == 0)
                                f = 7;
                            else
                                f--;
                        }   
                        
                    }   
                    else
                    {
                        int numbers[4] = {24,36,66,129};

                        //Switch number 2
                        if(PORTFbits.RF4 == 1)
                        {
                            //Switch number 3 == 0 --> go outward 
                            if(PORTDbits.RD15 == 0)
                            {
                                stringUp = "mode 2:";
                                if(Delay == slowDelay){
                                    stringDown = "Counter Outward Slow";
                                    lenDown = 20;
                                }
                                else{
                                    stringDown = "Counter Outward Fast";
                                    lenDown = 20;
                                }
                                lenUp = 7;
                                
                                PORTA = numbers[f];
                                if(f == 3)
                                    f = 0;
                                else
                                    f++;

                            }   
                            //Switch number 3 == 1 --> go inward 
                            else
                            {
                                stringUp = "mode 2:";
                                if(Delay == slowDelay){
                                    stringDown = "Counter Inward Slow";
                                    lenDown = 19;
                                }
                                else{
                                    stringDown = "Counter Inward Fast";
                                    lenDown = 19;
                                }
                                lenUp = 7;
                                
                                PORTA = numbers[f];
                                if(f == 0)
                                    f = 3;
                                else
                                    f--;
                            }   
                        }
                    }
                }
                }

            //Switch number 5 (if defined as 1 - stop the iteration)
            }else
            {
                stringUp = "Mode 5:";
                stringDown = "Halt";
                lenUp = 7;
                lenDown = 4;
            }

        // Switch number 7 stop the program and print on the screen without moving
        }else
        {   
            PORTA = 0x00;
            stringUp = "Mode 7:";
            stringDown = "Exit";
            lenUp = 7;
            lenDown = 4;
            startingPlace = 0xc0;
            function1(stringUp,stringDown,lenUp,lenDown, &startingPlace); // print the last thing on the screen and break
            break;
        }

        function1(stringUp,stringDown,lenUp,lenDown, &startingPlace);
        for(int i = 0 ; i < Delay ; i++);
    }

}

void busy(void)
     {
        char RD,RS;
        int STATUS_TRISE;
        int portMap;
        RD=PORTDbits.RD5;
        RS=PORTBbits.RB15;
        STATUS_TRISE=TRISE;
	    PORTDbits.RD5 = 1;//w/r
	    PORTBbits.RB15 = 0;//rs 
        portMap = TRISE;
	    portMap |= 0x80;
	    TRISE = portMap;
        do
        {
            PORTDbits.RD4=1;//enable=1
            PORTDbits.RD4=0;//enable=0
        }
        while(PORTEbits.RE7); // BF ?????
            PORTDbits.RD5=RD; 
            PORTBbits.RB15=RS;
            TRISE=STATUS_TRISE;  

    }

void function1(char* stringUp, char* stringDown, int lenUp, int lenDown, int* startingPlace){
    int i,j,k;
    int place = *startingPlace;
   
    PORTBbits.RB15=0;//rs=0
    PORTDbits.RD5=0;//w=0
    PORTE=0x01;
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
    busy();
        
    for(j=0;j<1;j++)
    {  
        PORTBbits.RB15=0;//rs=0
        PORTDbits.RD5=0;//w=0
   
  
        PORTE=0x80;
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    
    
    
        PORTBbits.RB15=1;//rs=1
        PORTDbits.RD5=0;//w=0
    
        for(i=0;i<lenUp;i++){
            PORTE=stringUp[i];
            PORTDbits.RD4=1;
            PORTDbits.RD4=0;
            //for(j=0;j<32000;j++);
            busy();    
 
        }
        PORTBbits.RB15=0;//rs=0
        PORTDbits.RD5=0;//w=0
   
        PORTE = place;
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    
        PORTBbits.RB15=1;//rs=1
        PORTDbits.RD5=0;//w=0
    
        for(i=0;i<lenDown;i++)
        {
            PORTE=stringDown[i];
            PORTDbits.RD4=1;
            PORTDbits.RD4=0;
            busy();
   
        }   

        *startingPlace = *(startingPlace) + 1;
        if(*startingPlace == 0xd5){
            *startingPlace = 0xb9;
        }

    }
}
