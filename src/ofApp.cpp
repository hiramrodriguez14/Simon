#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	//Let's create our buttons
	RedButton = new Button(ofGetWindowWidth()/2-20,ofGetWindowHeight()/2-260,302,239,"images/RedButton.png","sounds/RedButton.mp3");
	BlueButton = new Button(ofGetWindowWidth()/2+35,ofGetWindowHeight()/2-10,236,290,"images/BlueButton.png","sounds/BlueButton.mp3");
	YellowButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2+40,287,239,"images/YellowButton.png","sounds/YellowButton.mp3");
	GreenButton = new Button(ofGetWindowWidth()/2-260,ofGetWindowHeight()/2-260,234,294,"images/GreenButton.png","sounds/GreenButton.mp3");
	CompButton = new Button(ofGetWindowWidth()/2-500,ofGetWindowHeight()/2+280,150,100,"images/CompButton.png","sounds/EnterSound.mp3");
	ResetButton = new Button(ofGetWindowWidth()/2-500,ofGetWindowHeight()/2+150,100,100,"images/resetButton.png","sounds/KeySound.mp3");
	MultiplayerButton= new Button(ofGetWindowWidth()/2+400,ofGetWindowHeight()/2+280,100,100,"images/MultiplayerMode.png","sounds/EnterSound.mp3");


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

	//Font for the game mode
	font.load("fonts/extra_bold.ttf", 36);//<-The font path and thickness

	//Font for instruction
	myFont.load("fonts/bold.ttf",10);
}
//--------------------------------------------------------------
void ofApp::update(){

	// We will tick the buttons, aka constantly update them
	// while expecting input from the user to see if anything changed
	CompButton->tick();
	MultiplayerButton->tick();
	RedButton->tick();
	GreenButton->tick();
	BlueButton->tick();
	YellowButton->tick();

	// If the amount of user input equals the sequence limit
	// that means the user has successfully completed the whole
	// sequence and we can proceed with the next level
	if (gameState == PlayerInput && userIndex == sequenceLimit) {
		generateSequence();
		userIndex = 0;
		showingSequenceDuration = 0;
		gameState = PlayingSequence;
	}
	if (gameState == PlayerOneInput && p1Index == p1limit) {
		generateSequence();
		p1Index = 0;
		showingSequenceDuration = 0;
		gameState = PlayerTwoTurn;
	}
	if(gameState == PlayerTwoInput && p2Index == p2limit){
		generateSequence();
		p2Index = 0;
		showingSequenceDuration = 0;
		gameState = PlayerOneTurn;
	}

	// This will take care of turning off the lights after a few ticks
	// so that they don't stay turned on forever or too long
	if (lightDisplayDuration > 0) {
		lightDisplayDuration--;
		if (lightDisplayDuration <= 0) {
			lightOff(RED);
			lightOff(BLUE);
			lightOff(YELLOW);
			lightOff(GREEN);
		}
	}
	//This will replay the sequence when entering replay mode
	if (gameState == ReplayMode) {
		replaySequence();
	}

	//This defines the variable for the highscores and keep them updated, note that a minus 1 
	//is implemented because limits are 1 default
	p1Score = "Player 1 Highscore: " + ofToString(p1limit - 1);
    p2Score = "Player 2 Highscore: " + ofToString(p2limit - 1);
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
	//This will render Multiplayer and Free mode buttons
	if(!idle && gameState == StartUp){
		CompButton->render();
		MultiplayerButton->render();
	}

	
	//This whole if statement will take care of showing
	//the sequence to the user before accepting any input -> for any game mode <-

	//Classic mode
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
	//Multiplayer
	if(gameState == PlayerOneTurn){
		showingSequenceDuration++;
		if(showingSequenceDuration == 120){
			color = p1Sequence[p1Index];
			lightOn(color);
			lightDisplayDuration = 30;
		}

		if(showingSequenceDuration == 140){
			lightOff(color);
			showingSequenceDuration = 60;
			p1Index++;
		}
		if(p1Index == p1limit){
			lightOff(color);
			p1Index = 0;
			gameState = PlayerOneInput;
		}
	}
	if(gameState == PlayerTwoTurn){
		showingSequenceDuration++;
		if(showingSequenceDuration == 120){
			color = p2Sequence[p2Index];
			lightOn(color);
			lightDisplayDuration = 30;
		}
		
		if(showingSequenceDuration == 140){
			lightOff(color);
			showingSequenceDuration = 60;
			p2Index++;
		}
		if(p2Index == p2limit){
			lightOff(color);
			p2Index = 0;
			gameState = PlayerTwoInput;
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

	//Basically these control when and what messages will appear on screen
	if(!idle && gameState==StartUp){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		myFont.drawString("FREEMODE!",/*width*/2+47,/*height*/2+670);
		myFont.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
	}else if(!idle && gameState==FreeMode){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		myFont.drawString("PRESS 'r' TO RECORD YOUR SEQUENCE OR\n     PRESS 'p' TO REPLAY SEQUENCE!",/*width*/2+47,/*height*/2+670);	
	}else if(!idle && gameState==RecordMode){
		// font.drawString("MULTIPLAYER!",/*width*/2+915,/*height*/2+670);
		myFont.drawString("PLAY A SEQUENCE AND PRESS 'r' AGAIN TO STOP THE SEQUENCE!",/*width*/2+10,/*height*/2+670);
		
	}
	else if(!idle && (gameState == PlayerOneTurn || gameState == PlayerOneInput)){
        myFont.drawString("PLAYER 1 TURN TO PLAY THE SEQUENCE!",/*width*/2+47,/*height*/2 + 670);
    }
    else if(!idle && (gameState == PlayerTwoInput || gameState == PlayerTwoTurn)){
        myFont.drawString("PLAYER 2 TURN TO PLAY THE SEQUENCE!",/*width*/2+47,/*height*/2 + 670);
        
    }
	if(!idle && (gameState==PlayingSequence || gameState==PlayerInput)){
		font.drawString("Classic Mode", 2+350, 2 + 70);
	}
	if(!idle && (gameState==FreeMode || gameState==RecordMode ||gameState==ReplayMode)){
		font.drawString("Free Mode", 2+390, 2 + 70);
	}
	if(!idle && (gameState==PlayerOneInput|| gameState==PlayerTwoInput || gameState== PlayerOneTurn || gameState == PlayerTwoTurn)){
		font.drawString("Multiplayer", 2+360, 2 + 70);
	}
	if(!idle && !gameState==StartUp){
		myFont.drawString("PRESS BACKSPACE KEY TO GO TO MAIN MENU",2+720, 2 + 750);
	}
	//Highscores
	if(!idle && (gameState==PlayerOneInput|| gameState==PlayerTwoInput || gameState== PlayerOneTurn || gameState == PlayerTwoTurn)){
        myFont.drawString(p1Score,20, 20);
        myFont.drawString(p2Score,20, 40);
 }
}

	
//--------------------------------------------------------------
void ofApp::replaySequence() {
  //Sequence for Free Mode
    // Check if it's time to replay the next button press
    int currentTime = ofGetElapsedTimeMillis();
    if (replayIndex < recordedSequence.size() && currentTime - lastReplayTime >= 1000) {
        // Replay the next button press
        color = recordedSequence[replayIndex];
        lightOn(color);
		lightDisplayDuration = 15;
        // Update variables for the next replay
        replayIndex++;
        lastReplayTime = currentTime;
    }

    // Check if all button presses have been replayed
    if (replayIndex >= recordedSequence.size()) {
        // Reset variables and set display duration
        replayIndex = 0;
		gameState=FreeMode;
    }

}

//--------------------------------------------------------------
void ofApp::GameReset(){
	//This function will reset the game to its initial state
	p1limit=1;
	p2limit=1;
	lightOff(RED);
	lightOff(BLUE);
	lightOff(YELLOW);
	lightOff(GREEN);
	Sequence.clear();
	recordedSequence.clear();
	p1Sequence.clear();
	p2Sequence.clear();
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

	if(p1turn){
		generateSequence();
		p1Index = 0;
		p2Index = 0;
		gameState = PlayerOneTurn;
		showingSequenceDuration = 0;
	}
}

//--------------------------------------------------------------
void ofApp::generateSequence(){

	//This function will generate a random number between 0 and 3
	//of random doesnt include 4 so its between 0 and 3
	if(NormalPlay){
		int random = ofRandom(4);
	
	//Depending on the random number, we will add a button to the sequence
	//Classic Mode
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
	//Multiplayer
	if(p1turn || (gameState == PlayerOneInput)){
		int random = ofRandom(4);

		if (random == 0){
			p1Sequence.push_back(RED);
		}
		else if(random == 1){
			p1Sequence.push_back(BLUE);
		}
		else if(random == 2){
			p1Sequence.push_back(GREEN);
		}
		else if(random == 3){
			p1Sequence.push_back(YELLOW);
		}

		p1limit=p1Sequence.size();
	}
	if((gameState== PlayerTwoInput || firstrun) && (count < 2 || count > 2)){
		int random = ofRandom(4);

		if(random == 0){
			p2Sequence.push_back(RED);
		}

		else if(random == 1){
			p2Sequence.push_back(BLUE);
		}
		else if(random == 2){
			p2Sequence.push_back(GREEN);
		}
		else if(random == 3){
			p2Sequence.push_back(YELLOW);
		}
		p2limit=p2Sequence.size();
	}
	p1turn = false;
	firstrun = false;
	count++;
}
//--------------------------------------------------------------
bool ofApp::checkUserInput(Buttons input){
	//This function will varify if the user input matches the color
	//of the sequence at the current index
	if(NormalPlay){
		if(Sequence[userIndex] == input){
			return true;
		}
		else{
			return false;
		}
	}
	else if(gameState == PlayerOneInput){
		if(p1Sequence[p1Index] == input){
			return true;
		}
		else{
			return false;
		}
	}
	else if(gameState == PlayerTwoInput){
		if(p2Sequence[p2Index] == input){
			return true;
		}
		else{
			return false;
		}
	}
	return false;
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
		GameReset();
		if (gameState == StartUp){
			NormalPlay = true;
			GameReset();	
			CompButton->playSound();
		}
		//resets multiplayer mode with spacebar
		else if(multover){
			multover = false;
			p1turn = true;
			GameReset();
		}
	}
	//reset variables going back to the main screen
	if((!idle) && key == OF_KEY_BACKSPACE){
		multover = false;
		recordedSequence.clear();
		count = 1;
		p1Sequence.clear();
		p2Sequence.clear();
		NormalPlay = false;
		p1turn = false;
		FreePlay = false;
		gameState = StartUp;
		NormalPlay=false;
		FreePlay=false;
		ResetButton->playSound();
		firstrun = true;
	}
	if (!idle) {
        // Check if 'r' key is pressed
        if (tolower(key) == 'r') {
            if (gameState == FreeMode) {
                // Start recording mode
				ResetButton->playSound();
                gameState = RecordMode;
                recordedSequence.clear(); // Clear previous recording
            } else if (gameState == RecordMode) {
                // End recording mode
				ResetButton->playSound();
                gameState = FreeMode;
            }
        }
        // Check if 'p' key is pressed
        else if (tolower(key) == 'p') {
            if (gameState == FreeMode && recordedSequence.size() > 0) {
                // Start replay mode
				ResetButton->playSound();
                gameState = ReplayMode;
            }
        }
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
	//freemode button check
		if(CompButton->wasPressed()){
			CompButton->playSound();
			FreePlay = true;
			GameReset();

		}
	//multiplayer button check
		if(MultiplayerButton->wasPressed()){
			MultiplayerButton->playSound();
			p1turn=true;
			GameReset();

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
//checks player ones input and checks if it was correct
	if(!idle && gameState == PlayerOneInput){
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		YellowButton->setPressed(x,y);
		GreenButton->setPressed(x,y);

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
		else if(GreenButton->wasPressed()){
			color = GREEN;
			lightOn(color);
			lightDisplayDuration = 15;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
			lightOn(color);
			lightDisplayDuration = 15;
		}
			if(checkUserInput(color)){
				p1Index++;
			}
			else{
				multover = true;
				gameState = GameOver;
			}
	}
	//checks player twos inputs and verifies if correct
	if(!idle && gameState == PlayerTwoInput){
		RedButton->setPressed(x,y);
		BlueButton->setPressed(x,y);
		GreenButton->setPressed(x,y);
		YellowButton->setPressed(x,y);

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
		else if(GreenButton->wasPressed()){
			color = GREEN;
			lightOn(color);
			lightDisplayDuration = 15;
		}
		else if(YellowButton->wasPressed()){
			color = YELLOW;
			lightOn(color);
			lightDisplayDuration = 15;


		}
			if(checkUserInput(color)){
				p2Index++;
			}
			else{
				multover = true;
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
