#pragma once
#include <vector>
#include <raylib.h>
#include <string>

class Animation {
    private:
        int frameCount;
        float duration;
        float rotation;
        float scale;
        int currentDrawFrameIndex = 0;
        double lastFrameTime;
        float timeBetweenFrames;
        bool isLooping;
        Camera2D* camera;
    public:
        bool playing = true;
        bool isReverse = false;
        bool inWorld = true;
        Vector2 position;
        std::vector<Texture2D> frames;

        Animation(Camera2D *cam, Vector2 pos, float targetDuration, std::vector<std::string> allFrames, bool loop, float _rotation = 0.0f, float _scale = 1.0f){
            camera = cam;
            position = pos;
            duration = targetDuration;
            rotation = _rotation;
            scale = _scale;

            for (std::string fileName : allFrames){
                frames.emplace_back(LoadTexture(fileName.c_str()));
            }
            frameCount = frames.size();
            isLooping = loop;

            timeBetweenFrames = duration / frameCount;
        }

        bool Update() { // Return true if animation is finished
            if (!playing){return true;}
            double currentTime = GetTime();
            // Check if enough time has passed to advance the frame
            if (currentTime - lastFrameTime >= timeBetweenFrames) {
                lastFrameTime = currentTime;
                currentDrawFrameIndex++;

                if (currentDrawFrameIndex >= frameCount) { // At end of animation
                    if (isLooping == false) {
                        currentDrawFrameIndex = frameCount - 1; // Stay on the last frame
                        return true;
                    }
                    currentDrawFrameIndex = 0; // Reset to first frame
                }
            }
            return false; // Animation is still playing
        }

        void Stop(){
            playing = false;
            currentDrawFrameIndex = frameCount - 1;
        }

        void Play(bool loop = false){
            isLooping = loop;
            currentDrawFrameIndex = 0;
            playing = true;
        }

        void Draw() {
            if (!playing){return;}
            if (inWorld){BeginMode2D(*camera);}
            if (isReverse == false){
                DrawTextureEx(frames.at(currentDrawFrameIndex), Vector2{position.x, position.y}, rotation, scale, WHITE);
            }
            else{
                DrawTextureEx(frames.at(frameCount-1 - currentDrawFrameIndex), Vector2{position.x, position.y}, rotation, scale, WHITE);
            }
            if (inWorld){EndMode2D();}
        }
};

class DoorButtons{
    private:
        Sound doorNoise = LoadSound("res/aud/door.wav");
        std::vector<Texture2D> leftDoorButtonStates = {
            LoadTexture("res/tex/lButtonsAOff.png"),
            LoadTexture("res/tex/lButtonsLOn.png"),
            LoadTexture("res/tex/lButtonsDOn.png"),
            LoadTexture("res/tex/lButtonsAOn.png"),
        };
        std::vector<Texture2D> rightDoorButtonStates = {
            LoadTexture("res/tex/rButtonsAOff.png"),
            LoadTexture("res/tex/rButtonsLOn.png"),
            LoadTexture("res/tex/rButtonsDOn.png"),
            LoadTexture("res/tex/rButtonsAOn.png"),
        };
        Vector2 leftDoorButtonPos = {0, 200};
        Vector2 rightDoorButtonPos = {1500, 200};

        float doorButtonSize = 60;
        Vector2 rDoorOff = {15, 50};
        Vector2 lDoorOff = {25, 50};
        float buttonGap = 18;
        Rectangle rightDoor_DoorButton = Rectangle{rightDoorButtonPos.x + rDoorOff.x, rightDoorButtonPos.y + rDoorOff.y, doorButtonSize, doorButtonSize};
        Rectangle rightDoor_LightButton = Rectangle{rightDoorButtonPos.x + rDoorOff.x, rightDoorButtonPos.y + rDoorOff.y + rightDoor_DoorButton.height + buttonGap, doorButtonSize, doorButtonSize};
        Rectangle leftDoor_DoorButton = Rectangle{leftDoorButtonPos.x + lDoorOff.x, leftDoorButtonPos.y + lDoorOff.y, doorButtonSize, doorButtonSize};
        Rectangle leftDoor_LightButton = Rectangle{leftDoorButtonPos.x + lDoorOff.x, leftDoorButtonPos.y + lDoorOff.y + leftDoor_DoorButton.height + buttonGap, doorButtonSize, doorButtonSize};

    public:
        Sound lightNoise = LoadSound("res/aud/BallastHumMedium2.wav");
        Camera2D *camera;
        Vector2 leftDoorToggles = {0, 0}; // x - Door y - Light | 1-on 0-off
        Vector2 rightDoorToggles = {0, 0};

        ~DoorButtons(){
            StopSound(doorNoise);
            StopSound(lightNoise);
            UnloadSound(doorNoise);
            UnloadSound(lightNoise);
        }

        void Update(){
            Vector2 mPos = GetScreenToWorld2D(GetMousePosition(), *camera);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                //Left door button clicking
                if (CheckCollisionPointRec(mPos, leftDoor_DoorButton)){
                    if (leftDoorToggles.x == 1){
                        leftDoorToggles.x = 0;
                    }else{
                        leftDoorToggles.x = 1;
                    }
                    PlaySound(doorNoise);
                }

                if (CheckCollisionPointRec(mPos, leftDoor_LightButton)){
                    if (leftDoorToggles.y == 1){
                        leftDoorToggles.y = 0;
                    }else{
                        leftDoorToggles.y = 1;
                        rightDoorToggles.y = 0;
                    }
                    PlaySound(lightNoise);
                }

                //Right door button clicking
                if (CheckCollisionPointRec(mPos, rightDoor_DoorButton)){
                    if (rightDoorToggles.x == 1){
                        rightDoorToggles.x = 0;
                    }else{
                        rightDoorToggles.x = 1;
                    }
                    PlaySound(doorNoise);
                }

                if (CheckCollisionPointRec(mPos, rightDoor_LightButton)){
                    if (rightDoorToggles.y == 1){
                        rightDoorToggles.y = 0;
                    }else{
                        rightDoorToggles.y = 1;
                        leftDoorToggles.y = 0;
                    }
                    PlaySound(lightNoise);
                }
            }

            //Loop light noise while light on
            if (leftDoorToggles.y == 1 || rightDoorToggles.y == 1){
                if (IsSoundPlaying(lightNoise) == false){
                    PlaySound(lightNoise);
                }
            }
            else if (leftDoorToggles.y == 0 && rightDoorToggles.y == 0)
            {
                StopSound(lightNoise);
            }
        }

        void Draw(){
            BeginMode2D(*camera);

            //LEFT DOOR DRAW
            if (leftDoorToggles.x == 1){//If left door is on
                if (leftDoorToggles.y == 1){ //Both light and door are on
                    DrawTexture(leftDoorButtonStates[3], leftDoorButtonPos.x, leftDoorButtonPos.y, WHITE);
                }
                else{
                    DrawTexture(leftDoorButtonStates[2], leftDoorButtonPos.x, leftDoorButtonPos.y, WHITE);
                }
            }
            else if (leftDoorToggles.y == 1){ // Otherwise only the light is on
                DrawTexture(leftDoorButtonStates[1], leftDoorButtonPos.x, leftDoorButtonPos.y, WHITE);
            }
            else{ // Or nothing is
                DrawTexture(leftDoorButtonStates[0], leftDoorButtonPos.x, leftDoorButtonPos.y, WHITE);
            }

            //RIGHT DOOR DRAW
            if (rightDoorToggles.x == 1){//If right door is on
                if (rightDoorToggles.y == 1){ //Both light and door are on
                    DrawTexture(rightDoorButtonStates[3], rightDoorButtonPos.x, rightDoorButtonPos.y, WHITE);
                }
                else{
                    DrawTexture(rightDoorButtonStates[2], rightDoorButtonPos.x, rightDoorButtonPos.y, WHITE);
                }
            }
            else if (rightDoorToggles.y == 1){ // Otherwise only the light is on
                DrawTexture(rightDoorButtonStates[1], rightDoorButtonPos.x, rightDoorButtonPos.y, WHITE);
            }
            else{ // Or nothing is
                DrawTexture(rightDoorButtonStates[0], rightDoorButtonPos.x, rightDoorButtonPos.y, WHITE);
            }
            EndMode2D();
        }
};