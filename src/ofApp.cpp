#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//Let's create our buttons
	RedButton = new Button(ofGetWindowWidth()/2-20,ofGetWindowHeight()/2-260,302,239,"images/RedButton.png","sounds/RedButton.mp3");
	BlueButton = new Button(ofGetWindowWidth()/2+35,ofGetWindowHeight()/2-10,236,290,"images/BlueButton.png","sounds/BlueButton.mp3");
	YellowButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2+40,287,239,"images/YellowButton.png","sounds/YellowButton.mp3");
	GreenButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2-260,234,294,"images/GreenButton.png","sounds/GreenButton.mp3");
	CompButton = new Button(ofGetWindowWidth()/2-500,ofGetWindowHeight()/2+280,150,100,"images/CompButton.png","sounds/CompButton.mp3");
	ResetButton = new Button(ofGetWindowWidth()/2-500,ofGetWindowHeight()/2+150,100,100,"images/resetButton.png","sounds/ResetButton.mp3");
	MultiplayerButton= new Button(ofGetWindowWidth()/2+400,ofGetWindowHeight()/2+280,100,100,"images/MultiplayerMode.png","sounds/ResetButton.mp3");


	//Load the glowing images for the buttons
	redLight.load("images/RedLight.png");
    blueLight.load("images/BlueLight.png");
    yellowLight.load("images/YellowLight.png");
    greenLight.load("images/GreenLight.png");

	//Load other images
	logo.load("images/Logo.png");
	logoLight.load("images/LogoLight.png");
	startUpScreen.load("images/StartScreen.png");
	gameOverScreen.load("images/GameOverScreen.png");

	//Load Red and Green Dots for phase two!!
	redDot.load("images/RedDot.png");
	greenDot.load("images/GreenDot.png");
	
    //Load Music
	backgroundMusic.load("sounds/BackgroundMusic.mp3");
	backgroundMusic.setLoop(true);
	backgroundMusic.play();

	//Initial State
	gameState = StartUp;

	//load font
	//font.load("font.ttf", 36);<el path de el font aqui segundo parametro es el thickness del font
	
}
//--------------------------------------------------------------
void ofApp::update(){

	//We will tick the buttons, aka constantly update them
	//while expecting input from the user to see if anything changed
	

	if(gameState == StartUp){
		CompButton->tick();
		MultiplayerButton->tick();
	}
	if(gameState == RecordMode){
		RedButton->tick();
		BlueButton->tick();
		GreenButton->tick();
		YellowButton->tick();}

	if(gameState == ReplayMode){
		RedButton->tick();
		BlueButton->tick();
		GreenButton->tick();
		YellowButton->tick();
	}
	if(gameState == PlayerInput){
		RedButton->tick();
		BlueButton->tick();
		YellowButton->tick();
		GreenButton->tick();

		//If the amount of user input equals the sequence limit
		//that means the user has successfully completed the whole
		//sequence and we can proceed with the next level
		if(userIndex == sequenceLimit){
			generateSequence();
			userIndex = 0;
			showingSequenceDuration = 0;
			gameState = PlayingSequence;
		}
	}

	//This will take care of turning on the lights after a few
	//ticks so that they dont stay turned on forever or too long
	if(lightDisplayDuration > 0){
		lightDisplayDuration--;
		if(lightDisplayDuration <= 0){
			lightOff(RED);
			lightOff(BLUE);
			lightOff(YELLOW);
			lightOff(GREEN);
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	//Create the background
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));

	//Draw the buttons
	ofSetColor(255,0,0);
	RedButton->render();
	BlueButton->render();
	YellowButton->render();
	GreenButton->render();
	//ResetButton->render();
	if(!idle && gameState == StartUp){
		CompButton->render();
		MultiplayerButton->render();
	}

	
	//This whole if statement will take care of showing
	//the sequence to the user before accepting any input
	if(gameState == PlayingSequence){
		showingSequenceDuration++;
		if(showingSequenceDuration == 120){
			color = Sequence[userIndex];
			lightOn(color);
			lightDisplayDuration = 30;
		}
	
		if(showingSequenceDuration == 140){
			lightOff(color);
			showingSequenceDuration = 60;
			userIndex++;
		}
		if(userIndex == sequenceLimit){
			lightOff(color);
			userIndex = 0;
			gameState = PlayerInput;
		}
	}


	//StartUP (You dont need to pay much attention to this)
	//(This is only to create a animation effect at the start of the game)
	if(gameState == StartUp){
		showingSequenceDuration++;
		startUpSequence(showingSequenceDuration);
	}

	//If the statements to see see if the buttons should be lit up
	//If they are then we will draw the glowing images on top of them
	if (RedButton->GetIsLightUp()) {
		redLight.draw(ofGetWindowWidth()/2-60, ofGetWindowHeight()/2-305, 376, 329);
	}
	if (BlueButton->GetIsLightUp()) {
		blueLight.draw(ofGetWindowWidth()/2+5, ofGetWindowHeight()/2-60, 309, 376);
	} 
	if (YellowButton->GetIsLightUp()) {
		yellowLight.draw(ofGetWindowWidth()/2-300, ofGetWindowHeight()/2+5, 374, 318);
	} 
	if (GreenButton->GetIsLightUp()) {
		greenLight.draw(ofGetWindowWidth()/2-307, ofGetWindowHeight()/2-295, 315, 356);
	}

	//Part of the Start Up
	if(logoIsReady){
		logo.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);
	}

	//Draw the game over screen
	if(gameState == GameOver){
		gameOverScreen.draw(0,0,1024,768);
	}

	//This will draw the "Press to Start" screen at the beginning
	else if(!idle && gameState == StartUp){
		startUpScreen.draw(20,0,1024,768);
	}
	
	//This will draw the red dot when being on record mode
	if(gameState==RecordMode){
		redDot.draw(ofGetWindowWidth()/2-545, ofGetWindowHeight()/2-400, 150, 100);
	}
	//This will draw the green dot when being on replay mode
	if(gameState==ReplayMode){
		greenDot.draw(ofGetWindowWidth()/2-500, ofGetWindowHeight()/2-370, 50, 50);
	}

	if(!idle && gameState==StartUp){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		ofDrawBitmapString("FREEMODE!",/*width*/2+47,/*height*/2+670);
		ofDrawBitmapString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
	}else if(!idle && gameState==FreeMode){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		ofDrawBitmapString("PRESS 'r' TO RECORD YOUR SEQUENCE!",/*width*/2+47,/*height*/2+670);	
	}else if(!idle && gameState==RecordMode){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		ofDrawBitmapString("PRESS 'r' AGAIN TO STOP THE SEQUENCE OR\n     PRESS 'p' TO REPLAY SEQUENCE!",/*width*/2+47,/*height*/2+670);
		
	}

	

}

	
//--------------------------------------------------------------
void ofApp::replaySequence() {
    for (int i = 0; i < recordedSequence.size(); i++) {
		color = recordedSequence[i];
		lightOn(color); // Simulate button press
		lightDisplayDuration = 30;
		ofSleepMillis(1000);
    }
}
//--------------------------------------------------------------
void ofApp::GameReset(){
	//This function will reset the game to its initial state
	lightOff(RED);
	lightOff(BLUE);
	lightOff(YELLOW);
	lightOff(GREEN);
	Sequence.clear();
	if(NormalPlay){
		generateSequence();
		userIndex = 0;
		gameState = PlayingSequence;
		showingSequenceDuration = 0;
	}

	if (FreePlay){
		userIndex = 0;
		gameState = FreeMode;
		showingSequenceDuration = 0;


	}
}

//--------------------------------------------------------------
void ofApp::generateSequence(){

	//This function will generate a random number between 0 and 3
	//of random doesnt include 4 so its between 0 and 3
	int random = ofRandom(4);
	
	//Depending on the random number, we will add a button to the sequence
	if(random == 0){
		Sequence.push_back(RED);
	}
	else if(random == 1){
		Sequence.push_back(BLUE);
	}
	else if(random == 2){
		Sequence.push_back(GREEN);
	}
	else if(random == 3){
		Sequence.push_back(YELLOW);
	}

	//We will adjust the sequence limit to the new size of the Sequence list
	sequenceLimit = Sequence.size();
}
//--------------------------------------------------------------
bool ofApp::checkUserInput(Buttons input){
	//This function will varify if the user input matches the color
	//of the sequence at the current index
	if(Sequence[userIndex] == input){
		return true;
	}
	else{
		return false;
	}
}
//--------------------------------------------------------------
void ofApp::lightOn(Buttons color){
	//This function will take care of toggling the "isLightUp" variable to
	//true for the button that matches the color, and also play the button sound
	if(color == RED){
		RedButton->toggleLightOn();
		 RedButton->playSound();
	}
	else if(color == BLUE){
		BlueButton->toggleLightOn();
		 BlueButton->playSound();
	}
	else if(color == YELLOW){
		YellowButton->toggleLightOn();
        YellowButton->playSound();
	}
	else if(color == GREEN){
		GreenButton->toggleLightOn();
		 GreenButton->playSound();
	}
}

//--------------------------------------------------------------
void ofApp::lightOff(Buttons color){
	//This function will take care of toggling the "isLightUp" variable to false
	if(color == RED){
		RedButton->toggleLightOff();
	}
	else if(color == BLUE){
		BlueButton->toggleLightOff();
	}
	else if(color == YELLOW){
		YellowButton->toggleLightOff();
	}
	else if(color == GREEN){
		GreenButton->toggleLightOff();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//As long as we're not in Idle OR the gameState is GameOver;
	//AND we press the SPACEBAR, we will reset the game
	if((!idle || gameState == GameOver) && tolower(key) == ' '){
		NormalPlay = true;
		GameReset();
	}
	if((!idle) && key == OF_KEY_BACKSPACE){
		recordedSequence.clear();
		NormalPlay = false;
		FreePlay = false;
		gameState = StartUp;
	}
	if((!idle && gameState == FreeMode)&& tolower(key) == 'r'){
		recordedSequence.clear();
		gameState = RecordMode;
	}
	if((!idle && gameState == RecordMode)&& tolower(key) == 'p'){
		gameState = ReplayMode;
		replaySequence();
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int mouseX, int mouseY ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//If we're not in Idle and the gameState equals PlayerInput,
	//We will pay attention to the mousePresses from the user
	if(!idle && gameState == StartUp){
		CompButton->setPressed(x,y);
		MultiplayerButton->setPressed(x,y);
	}
		if(gameState== StartUp && CompButton->wasPressed()){
			CompButton->playSound();
			FreePlay = true;
			GameReset();

		}

		if(MultiplayerButton->wasPressed()){
			gameState = PlayerOneTurn;
		}
	

	if(!idle && gameState == RecordMode ){

		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		GreenButton->setPressed(x,y);
		YellowButton->setPressed(x,y);

		if(RedButton->wasPressed()){
			color = RED;
			lightOn(color);
			lightDisplayDuration = 15;
			recordedSequence.push_back(color);

		}
		else if(BlueButton->wasPressed()){
			color = BLUE;
			lightOn(color);
			lightDisplayDuration = 15;
			recordedSequence.push_back(color);

		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
			lightOn(color);
			lightDisplayDuration = 15;
			recordedSequence.push_back(color);

		}
		else if(GreenButton->wasPressed()){
			color = GREEN;
			lightOn(color);
			lightDisplayDuration = 15;
			recordedSequence.push_back(color);
		}
	}
	if(!idle && gameState == PlayerInput){
		//We mark the pressed button as "pressed"
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		YellowButton->setPressed(x,y);
		GreenButton->setPressed(x,y);

		//We check which button got pressed
		if(RedButton->wasPressed()){
			color = RED;
			lightOn(color);
			lightDisplayDuration = 15;
		}
		else if(BlueButton->wasPressed()){
			color = BLUE;
			lightOn(color);
			lightDisplayDuration = 15;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
			lightOn(color);
			lightDisplayDuration = 15;
		}
		else if(GreenButton->wasPressed()){
			color = GREEN;
			lightOn(color);
			lightDisplayDuration = 15;
		}
			//If the user input is correct, we can continue checking
			if(checkUserInput(color)){
				userIndex++;
			}
			//If not, then we will terminate the game by 
			//putting it in the GameOver state.
			else{
				gameState = GameOver;
			}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
//--------------------------------------------------------------

//You may honestly ignore this function, shouldnt be something you need
//to change or anything. It's only for the start up animation. But ofc,
//If you wish to make something out of it or make it better, be my guest.
void ofApp::startUpSequence(int count){
	
	if(count < 200){
		GreenButton->toggleLightOn();
	}
	else if(count >= 200 && count < 260){
		GreenButton->toggleLightOff();
		RedButton->toggleLightOn();
	}
	else if(count >= 260 && count < 320){
		RedButton->toggleLightOff();
		BlueButton->toggleLightOn();
	}
	else if(count >= 320 && count < 380){
		BlueButton->toggleLightOff();
		YellowButton->toggleLightOn();
	}
	else if(count >= 380 && count < 440){
		YellowButton->toggleLightOff();
	}
	else if(count >= 440 && count < 500){
		GreenButton->toggleLightOn();
		RedButton->toggleLightOn();
		YellowButton->toggleLightOn();
		BlueButton->toggleLightOn();
	}
	else if(count >= 500 && count < 560){
		GreenButton->toggleLightOff();
		RedButton->toggleLightOff();
		YellowButton->toggleLightOff();
		BlueButton->toggleLightOff();
	}
	else if(count >= 560){
		showingSequenceDuration = 400;
	}

	//Logo Drawing
	if(logoIsReady && logoCounter > 0){
		logoCounter--;
		ofSetColor(256,256,256,logoCounter);
		logoLight.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);
		ofSetColor(256,256,256);
	}
	if((count > 375) && !logoIsReady){
		logoCounter++;

		ofSetColor(256,256,256,logoCounter);
		logoLight.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);

		ofSetColor(256,256,256,logoCounter);
		logo.draw(ofGetWindowWidth()/2-160,ofGetWindowHeight()/2-150,330,330);

		ofSetColor(256,256,256);
	}
	if(logoCounter >=255){
		logoIsReady = true;
		idle = false;
	}
}
