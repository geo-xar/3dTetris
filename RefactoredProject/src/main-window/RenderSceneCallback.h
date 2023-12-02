class RenderSceneCallback final
{
public:
    static void render();
    static void changeSize(int w, int h);
    static void renderScene();
    static void processNormalKeys(unsigned char key, int, int);
    static void processSpecialKeys(int key, int, int);
};