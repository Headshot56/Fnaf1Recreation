#pragma once
#include <raymath.h>
#include <iostream>
#include <vector>
#include "Animations.hpp"
#include "Ai.hpp"

class DisplayManager{
    private:
        Texture officeBG = LoadTexture("./res/tex/OfficeBG.png");
        Texture officeLeftLightBG = LoadTexture("res/tex/leftLight.png");
        Texture officeRightLightBG = LoadTexture("res/tex/rightLight.png");
        Texture camBar = LoadTexture("res/tex/camBar.png");
        std::vector<Texture> menuScreens = {LoadTexture("res/tex/menuScreen1.png"), LoadTexture("res/tex/menuScreen2.png"), LoadTexture("res/tex/menuScreen3.png"), LoadTexture("res/tex/menuScreen4.png")};
        std::vector<Texture> cameraScreens = { //The terrible, evil, no good, very bad, horrendous, big, fat, list with all of the textures for every camera shittily organized
            LoadTexture("res/tex/ShowStageReg.png"),
            LoadTexture("res/tex/ShowStageSecret.png")
        };
        std::vector<Texture> menuText = {LoadTexture("res/tex/newGame.png"), LoadTexture("res/tex/continue.png"), LoadTexture("res/tex/fnafTitle.png"), LoadTexture("res/tex/copyright.png")};
        Sound boopNoise = LoadSound("res/aud/boop.wav");
        Sound fanNoise = LoadSound("res/aud/Buzz_Fan_Florescent2.wav");
        Sound camOpeningNoise = LoadSound("res/aud/CAMERA_VIDEO_LOA_60105303.wav");
        Sound camClosingNoise = LoadSound("res/aud/put down.wav");
        Sound cameraLoopSound = LoadSound("res/aud/MiniDV_Tape_Eject_1.wav");
        Sound menuMusic = LoadSound("res/aud/darkness music.wav");
        std::vector<Sound> officeAmbience = {LoadSound("res/aud/ambience2.wav"), LoadSound("res/aud/ColdPresc B.wav"), LoadSound("res/aud/EerieAmbienceLargeSca_MV005.wav")};
        std::vector<Sound> stingers = {LoadSound("res/aud/circus.wav"), LoadSound("res/aud/pirate song2.wav"), LoadSound("res/aud/Laugh_Giggle_Girl_1.wav")};
        std::string currentScreen = "menu"; //menu, office, cameras
        float turnSpeed;
        Camera2D* camera;
        DoorButtons buttons;
        int currentAmbience = GetRandomValue(0, officeAmbience.size()-1);
        bool touchingCamButton = false;
        bool wantToSwap = false;
        int menuScreen = GetRandomValue(0, menuScreens.size()-1);
        int currentStinger = 0;
        bool staticPlayed = false;
    public:
        std::vector<Animation> animations;

        DisplayManager(float ts, Camera2D* cam){
            turnSpeed = ts;
            camera = cam;
            buttons.camera = camera;
            std::vector<std::string> fanFrames = {"res/tex/fan#1.png", "res/tex/fan#2.png", "res/tex/fan#3.png"};
            animations.emplace_back(Animation{camera, Vector2{780, 303}, 0.15, fanFrames, true});
            std::vector<std::string> rDoorFrames = {"res/tex/Rdoor#1.png", "res/tex/Rdoor#2.png", "res/tex/Rdoor#3.png", "res/tex/Rdoor#4.png", "res/tex/Rdoor#5.png", "res/tex/Rdoor#6.png", "res/tex/Rdoor#7.png", "res/tex/Rdoor#8.png", "res/tex/Rdoor#9.png", "res/tex/Rdoor#10.png", "res/tex/Rdoor#11.png", "res/tex/Rdoor#12.png", "res/tex/Rdoor#13.png", "res/tex/Rdoor#14.png", "res/tex/Rdoor#15.png", "res/tex/Rdoor#16.png"};
            animations.emplace_back(Animation{camera, Vector2{1275, 0}, 0.5, rDoorFrames, false});
            std::vector<std::string> lDoorFrames = {"res/tex/Ldoor#1.png", "res/tex/Ldoor#2.png", "res/tex/Ldoor#3.png", "res/tex/Ldoor#4.png", "res/tex/Ldoor#5.png", "res/tex/Ldoor#6.png", "res/tex/Ldoor#7.png", "res/tex/Ldoor#8.png", "res/tex/Ldoor#9.png", "res/tex/Ldoor#10.png", "res/tex/Ldoor#11.png", "res/tex/Ldoor#12.png", "res/tex/Ldoor#13.png", "res/tex/Ldoor#14.png", "res/tex/Ldoor#15.png", "res/tex/Ldoor#16.png"};
            animations.emplace_back(Animation{camera, Vector2{75, 0}, 0.5, lDoorFrames, false});
            std::vector<std::string> camFlipFrames = {"res/tex/camFlip#1.png", "res/tex/camFlip#2.png", "res/tex/camFlip#3.png", "res/tex/camFlip#4.png", "res/tex/camFlip#5.png", "res/tex/camFlip#6.png", "res/tex/camFlip#7.png", "res/tex/camFlip#8.png", "res/tex/camFlip#9.png", "res/tex/camFlip#10.png", "res/tex/camFlip#11.png"};
            animations.emplace_back(Animation{camera, Vector2{0, 0}, 0.25, camFlipFrames, false});
            std::vector<std::string> camStaticFrames = {"res/tex/static#1.png", "res/tex/static#2.png", "res/tex/static#3.png", "res/tex/static#4.png", "res/tex/static#5.png", "res/tex/static#6.png", "res/tex/static#7.png", "res/tex/static#8.png"};
            animations.emplace_back(Animation{camera, Vector2{0, 0}, 0.2, camStaticFrames, false});
            animations[0].Stop(); //Fan animation
            animations[1].Stop(); //Right door animation
            animations[2].Stop(); //Left door animation
            animations[3].Stop(); //Cam flip animation
            animations[4].Stop(); //Cam static animation
            animations[4].inWorld = false; //Cam static shouldnt be affected by camera position
            animations[1].holdLastFrame = true;
            animations[2].holdLastFrame = true;
        }

        ~DisplayManager(){
            UnloadTexture(officeBG);
            UnloadTexture(officeLeftLightBG);
            UnloadTexture(officeRightLightBG);
            UnloadTexture(camBar);
            for (Texture t : menuScreens){
                UnloadTexture(t);
            }
            for (Texture t : cameraScreens){
                UnloadTexture(t);
            }
            for (Texture t : menuText){
                UnloadTexture(t);
            }
            StopSound(fanNoise);
            StopSound(boopNoise);
            StopSound(camOpeningNoise);
            StopSound(camClosingNoise);
            StopSound(cameraLoopSound);
            UnloadSound(boopNoise);
            UnloadSound(fanNoise);
            UnloadSound(camOpeningNoise);
            UnloadSound(cameraLoopSound);
            UnloadSound(camClosingNoise);
            UnloadSound(menuMusic);
        }

        void OfficeUpdate(){
            float mouseX = GetMousePosition().x;

            Rectangle freddyNose = {670, 235, 10, 10};
            Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), *camera);
            if (CheckCollisionPointRec(worldMousePos, freddyNose) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                PlaySound(boopNoise);
            }

            if (IsSoundPlaying(officeAmbience[currentAmbience]) == false){
                currentAmbience = GetRandomValue(0, officeAmbience.size()-1);
                PlaySound(officeAmbience[currentAmbience]);
            }
            
            if (IsSoundPlaying(fanNoise) == false){
                SetSoundVolume(fanNoise, 0.1);
                PlaySound(fanNoise);
            }

            //Camera movement
            if (mouseX < 50){
                camera->target = Vector2Add(camera->target, Vector2{-turnSpeed * GetFrameTime(), 0});
            }
            else if (mouseX > screenWidth-50){
                camera->target = Vector2Add(camera->target, Vector2{turnSpeed * GetFrameTime(), 0});
            }

            if (camera->target.x < 0){
                camera->target.x = 0;
            }

            // Clamp side to side movement in a way that works with different screen sizes and camera zoom
            float visibleWidth = screenWidth / camera->zoom;
            float maxTargetX = officeBG.width - visibleWidth;
            if (camera->target.x > maxTargetX){
                camera->target.x = maxTargetX;
            }

            int pRButton = buttons.rightDoorToggles.x;
            int pLButton = buttons.leftDoorToggles.x;
            buttons.Update();

            if (buttons.rightDoorToggles.x - pRButton == 1){
                animations[1].isReverse = false;
                animations[1].Play();
            }else if (buttons.rightDoorToggles.x - pRButton == -1){
                animations[1].isReverse = true;
                animations[1].Play();
            }
            if (buttons.leftDoorToggles.x - pLButton == 1){
                animations[2].isReverse = false;
                animations[2].Play();
            }else if (buttons.leftDoorToggles.x - pLButton == -1){
                animations[2].isReverse = true;
                animations[2].Play();
            }

            //Pull up cameras
            if (GetMouseY() >= screenHeight-60){
                if (touchingCamButton == false){
                    touchingCamButton = true;
                    if (animations[3].playing == false){
                        animations[3].position = camera->target;
                        animations[3].isReverse = false;
                        PlaySound(camOpeningNoise);
                        animations[3].Play();
                        staticPlayed = false;
                    }
                    wantToSwap = true;
                }
            }
            else{
                touchingCamButton = false;
            }

            //Go back to menu when pressing q since escape closes the window
            if (IsKeyPressed(KEY_Q)){
                animations[0].Stop();
                StopSound(fanNoise);
                StopSound(officeAmbience[currentAmbience]);
                menuScreen = GetRandomValue(0, menuScreens.size()-1);
                currentScreen = "menu";
            }

            if (animations[3].playing == false && wantToSwap){
                animations[0].Stop();
                StopSound(fanNoise);
                SetSoundVolume(officeAmbience[currentAmbience], 0.2); //Can still hear bg audio over cams but quieter
                SetSoundVolume(stingers[currentStinger], 0.2);
                currentScreen = "cameras";
                wantToSwap = false;
            }
        }

        void CameraUpdate(){
            if (IsSoundPlaying(cameraLoopSound) == false){
                PlaySound(cameraLoopSound);
            }

            //Put down cameras
            if (GetMouseY() >= screenHeight-60){
                if (touchingCamButton == false){
                    touchingCamButton = true;
                    if (animations[3].playing == false){
                        animations[3].position = camera->target;
                        animations[3].isReverse = true;
                        animations[4].Stop();
                        PlaySound(camClosingNoise);
                        animations[3].Play();
                    }
                    animations[0].Play(true);
                    StopSound(cameraLoopSound);
                    StopSound(camOpeningNoise); //Fast closing and opening of cams causes noise to play in office
                    SetSoundVolume(officeAmbience[currentAmbience], 1);
                    currentScreen = "office";
                }
            }
            else{
                touchingCamButton = false;
            }
        }
        
        void MenuUpdate(){
            if (IsSoundPlaying(menuMusic) == false){
                SetSoundVolume(menuMusic, 0.3);
                PlaySound(menuMusic);
            }
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                float scaleX = screenWidth / menuScreens[menuScreen].width;
                float scaleY = screenHeight / menuScreens[menuScreen].height;
                if (CheckCollisionPointRec(Vector2{float(GetMouseX()), float(GetMouseY())}, Rectangle{50, 100 + 50 + menuText[2].height*((scaleX < scaleY) ? scaleX : scaleY), menuText[1].width*scaleX, menuText[1].height*scaleY})){
                    StopSound(menuMusic);
                    animations[4].Stop();
                    animations[4].opacity = 1.0f;
                    animations[0].Play(true);
                    currentScreen = "office";
                }

            }
        }
        
        void MenuDraw(){
            //Scale texture to fit screen
            float scaleX = screenWidth / menuScreens[menuScreen].width;
            float scaleY = screenHeight / menuScreens[menuScreen].height;
            DrawTextureEx(menuScreens[menuScreen], Vector2{0, 0}, 0.0f, (scaleX < scaleY) ? scaleX : scaleY, WHITE);
            //Calculate the position below the title after scaling it to use as a reference for the button positions
            Vector2 titlePos = Vector2{50, 50 + menuText[2].height*((scaleX < scaleY) ? scaleX : scaleY)};
            DrawTextureEx(menuText[2], Vector2{50, 50}, 0.0f, (scaleX < scaleY) ? scaleX : scaleY, WHITE);
            DrawTextureEx(menuText[3], Vector2{screenWidth*0.5f-(menuText[3].width*scaleX)*0.5f, screenHeight-(menuText[3].height*scaleY)}, 0.0f, (scaleX < scaleY) ? scaleX : scaleY, WHITE);

            //Draw buttons
            DrawTextureEx(menuText[1], Vector2{50, titlePos.y + 100}, 0.0f, (scaleX < scaleY) ? scaleX : scaleY, WHITE);

            animations[4].scale = scaleX;
            animations[4].opacity = 0.25f;
            if (animations[4].playing == false){animations[4].Play();}
        }

        void OfficeDraw(){
            BeginMode2D(*camera);
            //Lights drawing
            if (buttons.leftDoorToggles.y == 1){
                SetSoundVolume(buttons.lightNoise, 1);
                DrawTexture(officeLeftLightBG, 0, 0, WHITE);
                //light flicker
                if (GetRandomValue(0, GetFPS()*0.2) == 1){SetSoundVolume(buttons.lightNoise, 0);DrawTexture(officeBG, 0, 0, WHITE);}
            }
            else if (buttons.rightDoorToggles.y == 1){
                SetSoundVolume(buttons.lightNoise, 1);
                DrawTexture(officeRightLightBG, 0, 0, WHITE);
                //light flicker
                if (GetRandomValue(0, GetFPS()*0.2) == 1){SetSoundVolume(buttons.lightNoise, 0);DrawTexture(officeBG, 0, 0, WHITE);}
            }
            else{
                DrawTexture(officeBG, 0, 0, WHITE);
            }
            EndMode2D();
            buttons.Draw();

            //Draw these animations here since draw order matters
            animations[1].Draw(); //Right door
            animations[2].Draw(); //Left door

            //Draw cambar
            DrawTextureEx(camBar, Vector2{screenWidth/2-(camBar.width*camera->zoom)/2, screenHeight-60}, 0.0f, camera->zoom, WHITE);
        }

        void CameraDraw(){
            //scale camera static to fit screen
            float scaleX = screenWidth / 1280; //magic number bad but 1280x720 is the resolution the static was made in
            animations[4].scale = scaleX;
            if (staticPlayed == false && animations[3].playing == false){
                animations[4].Play();
                staticPlayed = true;
            }
            
            camera->target = Vector2{0, 0};
            BeginMode2D(*camera);
            DrawTexture(cameraScreens[0], 0, 0, WHITE);
            EndMode2D();
        }

        void Update(){
            //randomly play spooky sounds
            if (GetRandomValue(0, GetFPS()*60) == 1 && (currentScreen == "office" || currentScreen == "cameras")){
                currentStinger = GetRandomValue(0, stingers.size()-1);
                PlaySound(stingers[currentStinger]);
            }

            if (currentScreen == "office"){OfficeUpdate();}
            if (currentScreen == "cameras"){CameraUpdate();}
            if (currentScreen == "menu"){MenuUpdate();}

            for (int i = 0; i < (int)animations.size(); i++){
                if (i == 3){//cam opening and closing anim
                    if (animations[i].Update()){
                        animations[i].Stop();
                    }
                }
                else{
                    animations[i].Update();
                }
            }
        }

        void Draw(){
            if (currentScreen == "office"){OfficeDraw();}
            if (currentScreen == "cameras"){CameraDraw();}
            if (currentScreen == "menu"){MenuDraw();}
            //Current issue im facing is that most but not all animations need to be drawn over everything so I might have to manually
            //draw each animation in the right order instead of looping through them all
            for (int i = 0; i < (int)animations.size(); i++){
                //Bad fix but ignore door animations in this loop since they are drawn in office draw
                if (i == 1 || i == 2){continue;}
                animations[i].Draw();
            }
        }
};