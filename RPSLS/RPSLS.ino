
#include "EEPROM.h"

const char* selection[] = {"Rock","Paper","Scissors","Lizard","Spock"};

class AI{
	public:
		AI(String);
		uint8_t play();
		void Init();
		float winLosseRatio();
		uint8_t victories;
		uint8_t losses;
		String name;
	private:
};

AI::AI(String com_name){
	name = com_name;
}

uint8_t AI::play(){
	uint8_t AI_input = (random(0,4.99));
	Serial.println( this->name+ "'s selection is "+selection[AI_input]);
	return AI_input;
}

float AI::winLosseRatio(){
	return float(victories)/float(losses);
}

void AI::Init(){
	this->victories = EEPROM.read(10);
	this->losses = EEPROM.read(11);

	Serial.print("Hello User My name is ");
	Serial.print(this->name);
	Serial.println(" and my Stats are:");
	Serial.println("victories = " + String(this->victories));
	Serial.println("Losses = " + String(this->losses));
	Serial.print("Win/Lose Ratio = ");
	Serial.println((*this).winLosseRatio());
}


AI Computer = AI("Kid");
bool multiplayer = false;
uint8_t rounds = 0;

void setup() {
	Serial.begin(115200);
	Serial.println("--- Start ---");
	Computer.Init();
	multiplayer = askMultiplayer();
	rounds = askRounds();
}

void loop(){
	
	int comp_score = 0;
	int user_score = 0;
	bool runing = true;

	while(runing){

		uint8_t user_inp;
		uint8_t comp_inp;
		
		if(multiplayer){
			comp_inp = sendMultiplayer(Computer.play());
			user_inp = recMultiplayer();
		}
		else{
			user_inp = askUser();
		 	comp_inp = Computer.play();			
		}

		int result = compute(comp_inp,user_inp);

		if(result == 1){
			comp_score += 1;
		}
		if(result == -1){
			user_score += 1;
		}

		Serial.print("User Score: ");
		Serial.println(user_score);

		Serial.print("Computer Score: ");
		Serial.println(comp_score);

		Serial.println("----");
		Serial.println(abs(comp_score- user_score));
		Serial.println((rounds/2)+1);
		Serial.println("----");

		if(abs(comp_score - user_score) >= (rounds/2) + 1){
			runing = false;
		}
	}

	Serial.println("--- --- --- ---");

	if(comp_score > user_score){
		Serial.println("Computer Win this Roud!");
		Computer.victories++;
	}
	else{
		Serial.println("User Win this Roud!");
		Computer.losses++;	
	}

	Serial.print("W/L Ratio = ");
	Serial.println(Computer.winLosseRatio());
}


String readString(){
	String output="";
	while(Serial.available())
	{
	  	output.concat(char(Serial.read()));
	  	delay(10);
	}
	return output;
}

uint8_t askUser(){
	uint8_t output = 6;
	Serial.print("Please insert your selection, the only valid commands are: ");
	for(int i=0; i<5 ;i++){
		Serial.print(selection[i]);
		if(i < 4){
			Serial.print(", ");
		}
	}
	Serial.println("");
	Serial.println("--- --- --- ---- ");
	while(true){
		if(Serial.available()){
			String inp = readString();
			for(int i=0;i<5;i++){
				if(inp == selection[i]){
					output = i;
					Serial.println("Your selection is: " + inp);
					return output;
				}
			}
			Serial.println(inp + " isn't a valid selection try again");
		}
	}
}

int compute(uint8_t comp, uint8_t user){

	char* connector[] = {"covers", "crushes", "vapourises","cuts","eats","disproves","decapitates","poisons","tie"};
	int output = 0;
	uint8_t pointer;

	if(comp == 0){			// Rock
		switch (user) {
			case 0:
				break;
		    case 1:
		    	output = -1; // Paper covers Rock
		    	pointer = 0;
		    	break;
		    case 2:
		    	output = 1;	// Rock crushes Scissors
		    	pointer = 1;
		    	break;
		    case 3:			
		    	output = 1;	// Rock crushes Lizzard
		    	pointer = 1;
		    	break;
		    case 4:
		    	output = -1; // Spock vaporize Lizzard
		    	pointer = 2;
		    	break;	
		}
	}
	else if(comp == 1){			// Paper
		switch (user) {
		    case 0:			// Rock
		    	output = 1;	// Paper covers Rock
		    	pointer = 0;
		    	break;
		    case 1:			// Paper
		    	break;	
		    case 2:			// Scissors
		    	output= -1;	// Scissors cuts paper
		    	pointer = 3;
		    	break; 
		    case 3:			// Lizard
		    	output= -1;	// Lizzar eats paper
		    	pointer = 4;
		    	break;
		    case 4:			// Spock
		    	output= 1;	// Paper disaprove spock
				pointer= 5;
				break;
		}
	}
	else if(comp == 2){			// Scissors
		switch (user) {
		    case 0:			// Rock
		    	output= -1;	// Rock crushes Scissors
		    	pointer=1;
		    	break;
		    case 1:			// Paper
		    	output= 1;	// Scissors cut paper;	
		    	pointer=3;
		    	break;
		    case 2:			// Scissors
		    	break;	
		    case 3:			// Lizard
		    	output= 1;	// Scissors decapitate Lizzard
		    	pointer=6;
		    	break;	
		    case 4:			// Spock
		    	output= -1;	// Spock crushes Scissors
		    	pointer=1;
		    	break;
		}
	}
	else if(comp == 3){			// Lizzard
		switch (user) {
		    case 0:			// Rock
		    	output= -1;	// Rock smashes Scissors
		    	pointer=1;
		    	break;
		    case 1:			// Paper
		    	output= 1;	// Lizzard eats paper;
		    	pointer=4;
		    	break;
		    case 2:			// Scissors
		    	output= -1;	// Scissors decapitate Lizzard
		    	pointer=6;
		    	break;
		    case 3:			// Lizard
		    	break;	
		    case 4:			// Spock
		    	output= -1;	// Lizzard poisons Spock
		    	pointer = 7;
		    	break;
		}
	}
	else if(comp == 4){			// Spock
		switch (user) {
		    case 0:			// Rock
		    	output= 1;	// Spock vaporise Rock
		    	pointer= 2;
		    	break;
		    case 1:			// Paper
		    	output= -1;	// Paper disaprove Spock;	
		    	pointer=5;
		    	break;
		    case 2:			// Scissors
		    	output= 1;	// Spock smash Scissors
		    	pointer = 1;
		    	break;
		    case 3:			// Lizard
		    	output= -1;	// Lizzard poisons Spock
		    	pointer=7;
		    	break;
		    case 4:			// Spock
		    	break;	
		}
	}

	if(output == -1){
		Serial.print(selection[user]);
		Serial.print(" ");
		Serial.print(connector[pointer]);
		Serial.print(" ");
		Serial.print(selection[comp]);
		Serial.println(". User Win!");
		return output;
	}
	if(output == 1){
		Serial.print(selection[comp]);
		Serial.print(" ");
		Serial.print(connector[pointer]);
		Serial.print(" ");
		Serial.print(selection[user]);
		Serial.println(". Computer Win!");
		return output;
	}
	if(output == 0){
		Serial.print(selection[comp]);
		Serial.println(" Tie!");
		return output;
	}
}


bool askMultiplayer(){
	Serial.println("It is a multiplayer match? yes/no");
	while(true){
	    if(Serial.available()){
	    	String inp = readString();
	    	if(inp == "yes"){
	    		return true;
	    	}
	    	else if(inp == "no"){
	    		return false;
	    	}
	    	else{
	    		Serial.println(inp + " is no a valid command, ty again");
	    	}
	    }
	}
}

uint8_t askRounds(){
	Serial.println("How many rouds? 1/3/5");
	while(true){
	    if(Serial.available()){
	    	String inp = readString();
	    	if(inp == "1"){
	    		return 1;
	    	}
	    	else if(inp == "3"){
	    		return 3;
	    	}
	    	else if(inp == "5"){
	    		return 5;
	    	}
	    	else{
	    		Serial.println(inp + " is no a valid command, ty again");
	    	}
	    }
	}
}

uint8_t sendMultiplayer(uint8_t choise){
	Serial.print("Hit enter to send the Computer Choise: ");
	Serial.println(selection[choise]);
	Serial.write(choise);
	return choise;
}

uint8_t recMultiplayer(){
	Serial.println("Waiting for the Remote Input");
	while(true){
		if(Serial.available()){
			uint8_t inp = uint8_t(Serial.read());
			if(inp < 5){
				Serial.println("The Remote User selection is: " + String(selection[inp]));
				Serial.flush();
				return inp;
			}
		}
	}
}