@startuml 3dTetris

class Game {
    +void start()
    +void pause()
    +void restart()
    +void stop()
    +void help()
    -State _state
    -Controller _controller
    -Logger _logger
}

class Logger {
    +void log()
}

class Grid {
    +void draw()
    -std::tuple<double,double,double> _position
    -std::tuple<double,double,double> _color
    -size_t _size
}

class Volume {
    +void draw()
    -std::vector<Cube> _cubes
    -std::tuple<double,double,double> _position
    -std::tuple<double,double,double> _color
}

class Cube {
    +void draw()
    -std::tuple<double,double,double> _position
    -std::tuple<double,double,double> _color
}

class Controller {
    -VolumeRandomGenerator _generator
    -std::vector<Volume> _volumes
    -Grid _grid
    -std::vector<LigthSource> _ligthSources
    -std::vector<Camera> _cameras
}

class LigthSource {
    -std::tuple<double,double,double> _position
}

class VolumeRandomGenerator {
    +Volume generate()
    -RandomEngine _engine
}

class Camera {
    +void zoomIn()
    +void zoomOut()
    -std::tuple<double,double,double> _position
    -std::vector<Input> _inputCollection
}

class Keyboard
class Mouse

interface Input

interface Drawable {
    +void draw()
}

Game --> "1" Controller
Game --> "1" Logger
Controller --> "1" VolumeRandomGenerator
Controller --> "1" Grid
Controller --> "*" Volume
Controller --> "*" LigthSource
Controller --> "*" Camera
Volume --> "*" Cube
Camera --> "*" Input

Grid --|> Drawable
Volume --|> Drawable
Cube --|> Drawable
Keyboard --|> Input
Mouse --|> Input

@enduml