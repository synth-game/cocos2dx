This repo contains our cocos2d custom version, which is use on Synth project.
We made modification on the engine to improve the keyboard event handling system.
Indeed when you pressed on a key in the previous version, the KEY_PRESS event was sent,
then plenty of KEY_RELEASE were sent while you handled the key.
Now the KEY_RELEASE event is sent when you really release the key.

Modified files are : 

event_dispatcher/CCEventKeyboard.h :
  - add a _isReleased member (l. 201, 205 and 211)

event_dispatcher/CCEventListenerKeyboard.cpp
  - in init() method, changer the "else" to an "else if(keyboardEvent->_isReleased)" (l. 84)

platform/win32/CCEGLView.cpp
  - add "GLFW_RELEASE == action" as a third parameter in the EventKeyboard constructor (l.343)


Works on Windows


To use the engine, clone this repo in the cocos2dx/ folder of your Cocos2d project workspace