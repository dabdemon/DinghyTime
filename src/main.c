//Declare and Import references
#include "pebble.h"
#include "pebble_fonts.h"


//#define TIME_FRAME        (GRect(0, 25, 144, 130)) 
	
//Definition for 4 layers
	//#define TIME_FRAME1       (GRect(0, 23, 142, 35)) 
	//#define TIME_FRAME2       (GRect(0, 55, 142, 35)) 
	//#define TIME_FRAME3       (GRect(0, 88, 142, 35)) 
	//#define TIME_FRAME4       (GRect(0, 121, 142, 35)) 

//Definition for 3 layers
	#define TIME_FRAME2       (GRect(0, 39, 142, 35)) 
	#define TIME_FRAME3       (GRect(0, 71, 142, 35)) 
	#define TIME_FRAME4       (GRect(0, 104, 142, 35))
	
	#define DATE_FRAME        (GRect(0, 3, 144, 30))
	#define JOKE_FRAME        (GRect(0, 154, 144, 14)) 
	
// Vibe pattern: ON for 400ms, OFF for 800ms:

	static const uint32_t const VibePatter[16] = { 400, 800, 400, 800, 400, 800, 400, 800, 400, 800, 400, 800, 400, 800, 400, 800 };
	
//Declare initial window	
	Window *my_window;    

//Define the layers
	TextLayer *date_layer;   		// Layer for the date
	//TextLayer *Time_Layer1; 		// Layer for the 1st line of the time
	TextLayer *Time_Layer2; 		// Layer for the 2nd line of the time
	TextLayer *Time_Layer3; 		// Layer for the 3rd line of the time
	TextLayer *Time_Layer4; 		// Layer for the 4th line of the time
	TextLayer *joke_layer;   		// Layer for the joke


//Define and initialize variables
	//FONTS
	GFont font_date;        // Font for date
	GFont font_time;        // Font for time
	GFont font_joke;        // Font for time


	//Date & Time	
	static char date_text[50];
	static char hour_text[] = "00";
	static char minute_text[] = "00";
	static char time_text[100];
	int ia = 0; 
	int ib = 0;
	int ic = 0;
	int id = 0;
	static char month_text[50];
	static char day_text[]="00";
	int ie = 0;
	int ig = 0;
	int ih = 0;

static const char *DINGHS[] = {
	NULL,
	"one ",
	"two ",
	"three ",
	"four ",
	"five ",
	"six ",
	"seven ",
	"eight ",
};

static char strAbout[]="about ";
static char strBell[]="bell ";
static char strBells[]="bells ";


static char strEarly[]= "early ";
static char strMid[] = "mid ";
static char strLate[] = "late ";

static char strjoke[]="it's five o'clock somewhere";

//Check if the bell needs to ding
bool blnDingBell = false;
int intNumBell = 0;

/*************/
/* DING BELL */
/*************/
static void vibes(int intVibes)
{

	
	
	VibePattern pat = {
						.durations = VibePatter,
						.num_segments = (intVibes * 2), //Number of bells (including silence)
	};
	vibes_enqueue_custom_pattern(pat);


	//Turn off the bell until next time
	blnDingBell=false;
	
}	


//************************//
// Capture the Tick event //
//************************//
void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{

/*
When the time is (either AM or PM): Display shows:
	12:16-12:45 one bell
	12:46-1:15 two bells
	1:16-1:45 three bells
	1:46-2:15 four bells
	2:16-2:45 five bells
	2:46-3:15 six bells
	3:16-3:45 seven bells
	3:46-4:15 eight bells
	
	4:16-4:45 one bell
	4:46-5:15 two bells
	5:16-5:45 three bells
	5:46-6:15 four bells
	6:16-6:45 five bells
	6:46-7:15 six bells
	7:16-7:45 seven bells
	7:46-8:15 eight bells
	
	8:16-8:45 one bell
	8:46-9:15 two bells
	9:16-9:45 three bells
	9:46-10:15 four bells
	10:16-10:45 five bells
	10:46-11:15 six bells
	11:16-11-45 seven bells
	11:46-12:15 eight bells
*/
	
/*
	July 1-10 "Early July"
	July 11-20 "Mid July"
	July 21-31 "Late July"
	August 1-10 "Early August"
*/
	
	if (units_changed & MINUTE_UNIT) 
	{

		//Format Date
		//strftime(date_text,sizeof(date_text),"%B %e",tick_time);
		strftime(day_text,sizeof(day_text),"%d",tick_time);
		strftime(month_text,sizeof(month_text),"%B",tick_time);
		ie = day_text[1]-'0';
		ig = day_text[0]-'0';
		ih = ie+(ig*10);
		
		//clean up the string
		memset(&date_text[0], 0, sizeof(date_text));
		
		if ((ih>0)&&(ih<11)){
			memcpy(&date_text, strEarly, strlen(strEarly)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		else if ((ih>10)&&(ih<21)){
			memcpy(&date_text, strMid, strlen(strMid)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		else{
			memcpy(&date_text, strLate, strlen(strLate)+1);
			strncat(date_text, month_text, strlen(month_text));
		}
		
		
		text_layer_set_text(date_layer,date_text); 

		
		//Format time
		
		strftime(hour_text, sizeof(hour_text), "%I", tick_time);
		strftime(minute_text, sizeof(minute_text), "%M", tick_time);
		
		ia = hour_text[1] - '0'; //hour
		if (hour_text[0]=='1'){ia=ia+10;}	
		
		ib = minute_text[1]-'0';
		ic = minute_text[0]-'0';
		id = ib+(ic*10); //minutes
		
		//clean up the string
		memset(&time_text[0], 0, sizeof(time_text));
		
		//ONE BELL
		if(((ia==12)&&(id>=16)&&(id<=45))||((ia==4)&&(id>=16)&&(id<=45))||((ia==8)&&(id>=16)&&(id<=45)))
		{
			//check if bell needs to ding
			if (intNumBell!=1){blnDingBell=true;
							   intNumBell = 1;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBell);
		}
		//TWO BELLS
		else if(((ia==12)&&(id>=46))||((ia==1)&&(id<=15))||((ia==4)&&(id>=46))||((ia==5)&&(id<=15))||((ia==8)&&(id>=46))||((ia==9)&&(id<=15)))
		{
			//check if bell needs to ding
			if (intNumBell!=2){blnDingBell=true;
							   intNumBell = 2;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//THREE BELLS
		else if(((ia==1)&&(id>=16)&&(id<=45))||((ia==5)&&(id>=16)&&(id<=45))||((ia==9)&&(id>=16)&&(id<=45)))
		{
			//check if bell needs to ding
			if (intNumBell!=3){blnDingBell=true;
							   intNumBell = 3;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//FOUR BELLS
		else if(((ia==1)&&(id>=46))||((ia==2)&&(id<=15))||((ia==5)&&(id>=46))||((ia==6)&&(id<=15))||((ia==9)&&(id>=46))||((ia==10)&&(id<=15)))
		{
			//check if bell needs to ding
			if (intNumBell!=4){blnDingBell=true;
							   intNumBell = 4;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//FIVE BELLS
		else if(((ia==2)&&(id>=16)&&(id<=45))||((ia==6)&&(id>=16)&&(id<=45))||((ia==10)&&(id>=16)&&(id<=45)))
		{
			//check if bell needs to ding
			if (intNumBell!=5){blnDingBell=true;
							   intNumBell = 5;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//SIX BELLS
		else if(((ia==2)&&(id>=46))||((ia==3)&&(id<=15))||((ia==6)&&(id>=46))||((ia==7)&&(id<=15))||((ia==10)&&(id>=46))||((ia==11)&&(id<=15)))
		{
			//check if bell needs to ding
			if (intNumBell!=6){blnDingBell=true;
							   intNumBell = 6;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//SEVEN BELLS
		else if(((ia==3)&&(id>=16)&&(id<=45))||((ia==7)&&(id>=16)&&(id<=45))||((ia==11)&&(id>=16)&&(id<=45)))
		{
			//check if bell needs to ding
			if (intNumBell!=7){blnDingBell=true;
							   intNumBell = 7;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//EIGHT BELLS
		else if(((ia==3)&&(id>=46))||((ia==4)&&(id<=15))||((ia==7)&&(id>=46))||((ia==8)&&(id<=15))||((ia==11)&&(id>=46))||((ia==12)&&(id<=15)))
		{
			//check if bell needs to ding
			if (intNumBell!=8){blnDingBell=true;
							   intNumBell = 8;}
			
			//text_layer_set_text(Time_Layer1, "");
			text_layer_set_text(Time_Layer2, strAbout);
			text_layer_set_text(Time_Layer3, DINGHS[intNumBell]);
			text_layer_set_text(Time_Layer4, strBells);
		}
		//ELSE (UNDEFINED)
		else{
			//text_layer_set_text(Time_Layer1, "there ");
			text_layer_set_text(Time_Layer2, "are not ");
			text_layer_set_text(Time_Layer3, "fucking ");
			text_layer_set_text(Time_Layer4, strBells);	
		}
		
		//Makes the Bell Ding (if needed)
		if(blnDingBell==true){vibes(intNumBell);}

	} //MINUTE CHANGES

} //HANDLE_TICK 



//****************************//
// Initialize the application //
//****************************//

void handle_init(void)
{

	//Create the main window
	my_window = window_create(); 
	window_stack_push(my_window, true /* Animated */);
	window_set_background_color(my_window, GColorBlack);

	
	
    font_time = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);
	 
	font_date = fonts_get_system_font(FONT_KEY_GOTHIC_18);
	font_joke = fonts_get_system_font(FONT_KEY_GOTHIC_14);


	//LOAD THE LAYERS

		//Display the Time layer
	/*
		Time_Layer1 = text_layer_create(TIME_FRAME1);
		text_layer_set_text_color(Time_Layer1, GColorWhite);
		text_layer_set_background_color(Time_Layer1, GColorClear);
		text_layer_set_font(Time_Layer1, font_time);
		text_layer_set_text_alignment(Time_Layer1, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer1)); 
	*/

		Time_Layer2 = text_layer_create(TIME_FRAME2);
		text_layer_set_text_color(Time_Layer2, GColorWhite);
		text_layer_set_background_color(Time_Layer2, GColorClear);
		text_layer_set_font(Time_Layer2, font_time);
		text_layer_set_text_alignment(Time_Layer2, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer2)); 
	
		Time_Layer3 = text_layer_create(TIME_FRAME3);
		text_layer_set_text_color(Time_Layer3, GColorWhite);
		text_layer_set_background_color(Time_Layer3, GColorClear);
		text_layer_set_font(Time_Layer3, font_time);
		text_layer_set_text_alignment(Time_Layer3, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer3)); 
	
		Time_Layer4 = text_layer_create(TIME_FRAME4);
		text_layer_set_text_color(Time_Layer4, GColorWhite);
		text_layer_set_background_color(Time_Layer4, GColorClear);
		text_layer_set_font(Time_Layer4, font_time);
		text_layer_set_text_alignment(Time_Layer4, GTextAlignmentRight);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(Time_Layer4)); 
	
		//Display the Date layer
		date_layer = text_layer_create(DATE_FRAME);
		text_layer_set_text_color(date_layer, GColorWhite);
		text_layer_set_background_color(date_layer, GColorClear);
		text_layer_set_font(date_layer, font_date);
		text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(date_layer)); 
	
		//Display the Joke layer
		joke_layer = text_layer_create(JOKE_FRAME);
		text_layer_set_text_color(joke_layer, GColorWhite);
		text_layer_set_background_color(joke_layer, GColorClear);
		text_layer_set_font(joke_layer,font_joke);
		text_layer_set_text_alignment(joke_layer, GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(joke_layer)); 

  		text_layer_set_text(joke_layer,strjoke);
	
	// Ensures time is displayed immediately (will break if NULL tick event accessed).
	  // (This is why it's a good idea to have a separate routine to do the update itself.)

		time_t now = time(NULL);
	  	struct tm *current_time = localtime(&now);
		handle_tick(current_time, MINUTE_UNIT);
		tick_timer_service_subscribe(MINUTE_UNIT, &handle_tick);
	


} //HANDLE_INIT



//**********************//
// Kill the application //
//**********************//
void handle_deinit(void)
{
  //text_layer_destroy(text_layer);

	//Unsuscribe services
	tick_timer_service_unsubscribe();

	//Deallocate layers
	//text_layer_destroy(Time_Layer1);
	text_layer_destroy(Time_Layer2);
	text_layer_destroy(Time_Layer3);
	text_layer_destroy(Time_Layer4);
	text_layer_destroy(date_layer);

	//Deallocate the main window
  	window_destroy(my_window);

} //HANDLE_DEINIT


//*************//
// ENTRY POINT //
//*************//
int main(void) 
{	
	handle_init();
	app_event_loop();
	handle_deinit();
}
