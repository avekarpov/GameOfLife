#include "GameOfLife.h"
#include <iostream>
#include <algorithm>


//Rules ================================================================================================================


const char *GameOfLife::World::Rules::OutOfRules::what() const noexcept
{
    return "Out of rules";
}

GameOfLife::World::Rules::Rules(const GameOfLife::World::Rules &rules)
{
    for (unsigned i = 0; i < 9; ++i)
    {
        this->b_[i] = rules.b_[i];
        this->s_[i] = rules.s_[i];
    }
}

GameOfLife::World::Rules &GameOfLife::World::Rules::operator=(const GameOfLife::World::Rules &rules)
{
    if (this == &rules)
    {
        return *this;
    }

    for (unsigned i = 0; i < 9; ++i)
    {
        this->b_[i] = rules.b_[i];
        this->s_[i] = rules.s_[i];
    }

    return *this;
}

void GameOfLife::World::Rules::b_set(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    this->b_[index] = true;
}

void GameOfLife::World::Rules::b_reset(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    this->b_[index] = false;
}

const bool &GameOfLife::World::Rules::b_test(unsigned int index) const
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    return this->b_[index];
}

bool &GameOfLife::World::Rules::b(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    return this->b_[index];
}

void GameOfLife::World::Rules::s_set(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    this->s_[index] = true;
}

void GameOfLife::World::Rules::s_reset(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    this->s_[index] = false;
}

const bool &GameOfLife::World::Rules::s_test(unsigned int index) const
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    return this->s_[index];
}

bool &GameOfLife::World::Rules::s(unsigned int index)
{
    if (9 <= index)
    {
        throw GameOfLife::World::Rules::OutOfRules();
    }

    return this->s_[index];
}


//World ================================================================================================================


const char *GameOfLife::World::OutOfMap::what() const noexcept
{
    return "Out of map";
}

GameOfLife::World::World()
{
    this->width_ = 0;
    this->height_ = 0;

    this->grid_ = nullptr;
    this->bufferGrid_ = nullptr;
}

GameOfLife::World::World(unsigned int width, unsigned int height)
{
    this->width_ = width;
    this->height_ = height;

    this->grid_ = new bool *[this->height_];
    this->bufferGrid_ = new bool *[this->height_];
    for (unsigned i = 0; i < this->height_; ++i)
    {
        this->grid_[i] = new bool[this->width_];
        this->bufferGrid_[i] = new bool[this->width_];
    }
}

GameOfLife::World::World(const World &world)
{
    this->width_ = world.width_;
    this->height_ = world.height_;

    this->grid_ = new bool *[this->height_];
    this->bufferGrid_ = new bool *[this->height_];
    for (unsigned i = 0; i < this->height_; ++i)
    {
        this->grid_[i] = new bool[this->width_];
        this->bufferGrid_[i] = new bool[this->width_];
        for (unsigned j = 0; j < this->width_; ++i)
        {
            this->grid_[i][j] = world.grid_[i][j];
            //this->bufferGrid_[i][j] = world.bufferGrid_[i][j];
        }
    }
}

GameOfLife::World &GameOfLife::World::operator=(const World &world)
{
    if (this == &world)
    {
        return *this;
    }

    for (unsigned i = 0; i < this->height_; ++i)
    {
        delete[] this->grid_[i];
        delete[] this->bufferGrid_[i];
    }
    delete[] this->grid_;
    delete[] this->bufferGrid_;

    this->width_ = world.width_;
    this->height_ = world.height_;

    this->grid_ = new bool *[this->height_];
    this->bufferGrid_ = new bool *[this->height_];
    for (unsigned i = 0; i < this->height_; ++i)
    {
        this->grid_[i] = new bool[this->width_];
        this->bufferGrid_[i] = new bool[this->width_];
        for (unsigned j = 0; j < this->width_; ++i)
        {
            this->grid_[i][j] = world.grid_[i][j];
            //this->bufferGrid_[i][j] = world.bufferGrid_[i][j];
        }
    }

    return *this;
}

void GameOfLife::World::resize(unsigned int width, unsigned int height)
{
    bool **newGrid = new bool *[height];
    bool **newBufferGrid = new bool *[height];
    for (unsigned i = 0; i < height; ++i)
    {
        newGrid[i] = new bool[width];
        newBufferGrid[i] = new bool[width];
    }

    if (width != 0 && height != 0 && this->width_ != 0 && this->height_ != 0)
    {
        for (unsigned i = std::min(height, this->height_) - 1; true; --i)
        {
            for (unsigned j = std::min(width, this->width_) - 1; true; --j)
            {
                newGrid[i][j] = this->grid_[i][j];

                if (j == 0)
                {
                    break;
                }
            }

            if (i == 0)
            {
                break;
            }
        }

        for (unsigned i = this->height_; i < height; ++i)
        {
            for (unsigned j = 0; j < this->width_; ++j)
            {
                newGrid[i][j] = false;
            }
        }

        for (unsigned i = 0; i < this->height_; ++i)
        {
            for (unsigned j = this->width_; j < width; ++j)
            {
                newGrid[i][j] = false;
            }
        }

        for (unsigned i = this->height_; i < height; ++i)
        {
            for (unsigned j = this->width_; j < width; ++j)
            {
                newGrid[i][j] = false;
            }
        }
    }

    for (unsigned i = 0; i < this->height_; ++i)
    {
        delete[] this->grid_[i];
        delete[] this->bufferGrid_[i];
    }
    delete[] this->grid_;
    delete[] this->bufferGrid_;

    this->width_ = width;
    this->height_ = height;

    std::swap(newGrid, this->grid_);
    std::swap(newBufferGrid, this->bufferGrid_);
}

const unsigned int &GameOfLife::World::width() const
{
    return this->width_;
}

const unsigned int &GameOfLife::World::height() const
{
    return this->height_;
}

GameOfLife::World::Rules &GameOfLife::World::rules()
{
    return this->rules_;
}

bool &GameOfLife::World::at(unsigned int row, unsigned int column)
{
    if (this->height_ <= row || this->width_ <= column)
    {
        throw GameOfLife::World::OutOfMap();
    }

    return this->grid_[row][column];
}

void GameOfLife::World::clear()
{
    for (unsigned i = 0; i < this->height_; ++i)
    {
        for (unsigned j = 0; j < this->width_; ++j)
        {
            this->grid_[i][j] = false;
        }
    }
}

void GameOfLife::World::fill_rand(unsigned int seed)
{
    srand(seed);

    for (unsigned i = 0; i < this->height_; ++i)
    {
        for (unsigned j = 0; j < this->width_; ++j)
        {
            this->grid_[i][j] = static_cast<bool>(rand() % 2);
        }
    }
}

void GameOfLife::World::inversion()
{
    for (unsigned i = 0; i < this->height_; ++i)
    {
        for (unsigned j = 0; j < this->width_; ++j)
        {
            this->grid_[i][j] = !this->grid_[i][j];
        }
    }
}

void GameOfLife::World::makeStep()
{
    unsigned count_neighboring_cells;

    for (unsigned i = 0; i < this->height_; ++i)
    {
        for (unsigned j = 0; j < this->width_; ++j)
        {
            count_neighboring_cells = 0;

            count_neighboring_cells += (i != 0 && j != 0 && this->grid_[i - 1][j - 1]);
            count_neighboring_cells += (i != 0 && j < this->width_ - 1 && this->grid_[i - 1][j + 1]);
            count_neighboring_cells += (i < this->height_ - 1 && j < this->width_ - 1 && this->grid_[i + 1][j + 1]);
            count_neighboring_cells += (i < this->height_ - 1 && j != 0 && this->grid_[i + 1][j - 1]);
            count_neighboring_cells += (i != 0 && this->grid_[i - 1][j]);
            count_neighboring_cells += (j < this->width_ - 1 && this->grid_[i][j + 1]);
            count_neighboring_cells += (i < this->height_ - 1 && this->grid_[i + 1][j]);
            count_neighboring_cells += (j != 0 && this->grid_[i][j - 1]);

            this->bufferGrid_[i][j] = ((!this->grid_[i][j] && this->rules_.b_test(count_neighboring_cells)) ||
                                       (this->grid_[i][j] && this->rules_.s_test(count_neighboring_cells)));
        }
    }

    std::swap(this->grid_, this->bufferGrid_);
}

std::ostream &operator<<(std::ostream &os, const GameOfLife::World &world)
{
    os << world.width_ << ' ' << world.height_ << std::endl;

    for (unsigned i = 0; i < world.height_; ++i)
    {
        for (unsigned j = 0; j < world.width_; ++j)
        {
            os << world.grid_[i][j] << ' ';
        }
        os << std::endl;
    }

    for (unsigned i = 0; i < 9; ++i)
    {
        os << world.rules_.b_test(i) << ' ';
    }
    for (unsigned i = 0; i < 9; ++i)
    {
        os << world.rules_.s_test(i) << ' ';
    }

    return os;
}

std::istream &operator>>(std::istream &is, GameOfLife::World &world)
{
    unsigned width, height;

    is >> width >> height;

    world.resize(width, height);

    for (unsigned i = 0; i < world.height_; ++i)
    {
        for (unsigned j = 0; j < world.width_; ++j)
        {
            is >> world.grid_[i][j];
        }
    }

    for (unsigned i = 0; i < 9; ++i)
    {
        is >> world.rules().b(i);
    }
    for (unsigned i = 0; i < 9; ++i)
    {
        is >> world.rules().s(i);
    }

    return is;
}

GameOfLife::World::~World()
{
    if (this->grid_ != nullptr)
    {
        for (unsigned i = 0; i < this->height_; ++i)
        {
            delete[] this->grid_[i];
            delete[] this->bufferGrid_[i];
        }

        delete[] this->grid_;
        delete[] this->bufferGrid_;
    }
}


//GameOfLife ===========================================================================================================


GameOfLife::GameOfLife(unsigned int width, unsigned int height, unsigned int scale) :
world_(width, height),
window_(sf::VideoMode(width * scale,height * scale),"Game of life")
{
    this->scale_ = scale;

    this->stepInterval_ = 50;
    this->isStop_ = false;
}

GameOfLife::World::Rules &GameOfLife::rules()
{
    return this->world_.rules();
}

void GameOfLife::run(bool grid)
{
    sf::Event event;

    sf::RectangleShape rectangleShape(sf::Vector2f(this->scale_, this->scale_));
    rectangleShape.setFillColor(sf::Color::Black);


    sf::RectangleShape cell(sf::Vector2f(this->scale_, this->scale_));
    cell.setFillColor(sf::Color::White);
    cell.setOutlineColor(sf::Color::Black);
    cell.setOutlineThickness(0.5);


    this->world_.rules().b_set(3);
    this->world_.rules().s_set(2);
    this->world_.rules().s_set(3);

    this->world_.fill_rand();

    while (this->window_.isOpen())
    {
        this->handleEvent_(event);
        
        this->window_.clear(sf::Color::White);

        if(grid)
        {
            for (unsigned i = 0; i < this->world_.height(); ++i)
            {
                for (unsigned j = 0; j < this->world_.width(); ++j)
                {
                    cell.setPosition(j * this->scale_, i * this->scale_);
                    this->window_.draw(cell);
                }
            }
        }


        for (unsigned i = 0; i < this->world_.height(); ++i)
        {
            for (unsigned j = 0; j < this->world_.width(); ++j)
            {
                if (this->world_.at(i, j))
                {
                    rectangleShape.setPosition(static_cast<float>(j * this->scale_),
                                               static_cast<float>(i * this->scale_));
                    this->window_.draw(rectangleShape);
                }
            }
        }

        this->window_.display();
    }
}

void GameOfLife::handleEvent_(sf::Event &event)
{
    sf::Vector2f mouse_position;

    while (this->window_.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                this->window_.close();

                break;
            }

            case sf::Event::KeyPressed:
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                    {
                        this->window_.close();

                        break;
                    }

                    case sf::Keyboard::Space:
                    {
                        this->isStop_ = !this->isStop_;

                        break;
                    }

                    case sf::Keyboard::F:
                    {
                        this->world_.fill_rand();

                        break;
                    }

                    case sf::Keyboard::C:
                    {
                        this->world_.clear();

                        break;
                    }

                    case sf::Keyboard::I:
                    {
                        this->world_.inversion();

                        break;
                    }

                    case sf::Keyboard::R:
                    {
                        char rule;
                        unsigned number;

                        std::cout << "Rules settings (to exit 'q')" << std::endl;
                        std::cout << "Rules:" << std::endl;

                        std::cout << "b: ";
                        for (unsigned i = 0; i < 9; ++i)
                        {
                            std::cout << '[' << i << ": " << this->world_.rules().b(i) << "] ";
                        }
                        std::cout << std::endl;

                        std::cout << "s: ";
                        for (unsigned i = 0; i < 9; ++i)
                        {
                            std::cout << '[' << i << ": " << this->world_.rules().s(i) << "] ";
                        }
                        std::cout << std::endl;

                        do
                        {
                            std::cout << "To change rule enter RULE(b/s) and NUMBER(0-8): ";

                            std::cin >> rule;
                            if (rule == 'q' || rule == 'Q')
                            {
                                std::cout << "Exit from Rules settings." << std::endl;
                                break;
                            }
                            std::cin.get();
                            number = static_cast<unsigned>(std::cin.get()) - 48;
                            while (rule != 'b' && rule != 's' || (9 <= number))
                            {
                                std::cout << "Wrong rule! Try again: ";
                                std::cin >> rule;
                                if (rule == 'q' || rule == 'Q')
                                {
                                    break;
                                }
                                std::cin.get();
                                number = static_cast<unsigned>(std::cin.get()) - 48;
                            }

                            if (rule == 'q' || rule == 'Q')
                            {
                                std::cout << "Exit from Rules settings." << std::endl;
                                break;
                            }

                            if (rule == 'b')
                            {
                                this->world_.rules().b(number) = !this->world_.rules().b(number);
                            }
                            else
                            {
                                this->world_.rules().s(number) = !this->world_.rules().s(number);
                            }

                            std::cout << "Rules changed:" << std::endl;

                            std::cout << "b: ";
                            for (unsigned i = 0; i < 9; ++i)
                            {
                                std::cout << '[' << i << ": " << this->world_.rules().b(i) << "] ";
                            }
                            std::cout << std::endl;

                            std::cout << "s: ";
                            for (unsigned i = 0; i < 9; ++i)
                            {
                                std::cout << '[' << i << ": " << this->world_.rules().s(i) << "] ";
                            }
                            std::cout << std::endl;
                        } while (rule != 'q' && rule != 'Q');

                        break;
                    }

                    case sf::Keyboard::Up:
                    {
                        if (this->stepInterval_ != 50)
                        {
                            this->stepInterval_ -= 50;
                        }

                        break;
                    }

                    case sf::Keyboard::Down:
                    {
                        if (this->stepInterval_ != 500)
                        {
                            this->stepInterval_ += 50;
                        }

                        break;
                    }

                    case sf::Keyboard::S:
                    {
                        std::string fpath;
                        std::ofstream fout;

                        char command;

                        std::cout << "Save settings (to exit 'q')" << std::endl;

                        std::cout << "Enter file path: ";
                        std::cin >> fpath;

                        if (fpath == "q" || fpath == "Q")
                        {
                            std::cout << "Exit from Save settings." << std::endl;
                            break;
                        }

                        if (fpath.back() == '/')
                        {
                            std::cout << "You entered a directory, the game will be saved to a GOF file." << std::endl;
                            fpath += "GOF";
                        }

                        while (std::ifstream(fpath).is_open())
                        {
                            std::cout << "The file already exists! Are you want to rewrite it? (y/n): ";
                            std::cin >> command;

                            if (command == 'q' || command == 'Q')
                            {
                                fpath = "q";
                                break;
                            }
                            else if (command == 'n' || command == 'N')
                            {
                                std::cout << "Enter file path: ";
                                std::cin >> fpath;

                                if (fpath.back() == '/')
                                {
                                    std::cout << "You entered a directory, the game will be saved to a GOF file." << std::endl;
                                    fpath += "GOF";
                                }
                            }
                            else if (command == 'y' || command == 'Y')
                            {
                                break;
                            }
                        }

                        if (fpath == "q" || fpath == "Q")
                        {
                            std::cout << "Exit from Save settings." << std::endl;
                            break;
                        }

                        fout.open(fpath);

                        fout << this->world_;

                        fout.close();

                        std::cout << "Exit from Save settings." << std::endl;

                        break;
                    }

                    case sf::Keyboard::O:
                    {
                        std::string fpath;
                        std::ifstream fin;

                        std::cout << "Open settings (to exit 'q')" << std::endl;

                        std::cout << "Enter file path: ";
                        std::cin >> fpath;

                        if (fpath == "q" || fpath == "Q")
                        {
                            std::cout << "Exit from Open settings." << std::endl;
                            break;
                        }

                        if (fpath.back() == '/')
                        {
                            std::cout << "You entered a directory, the GOF file will be opened." << std::endl;
                            fpath += "GOF";
                        }

                        while (!std::ifstream(fpath).is_open())
                        {
                            std::cout << "The file not found! Try again: ";
                            std::cin >> fpath;

                            if (fpath.back() == '/')
                            {
                                std::cout << "You entered a directory, the GOF file will be opened." << std::endl;
                                fpath += "GOF";
                            }

                            if (fpath == "q" || fpath == "Q")
                            {
                                break;
                            }
                        }

                        if (fpath == "q" || fpath == "Q")
                        {
                            std::cout << "Exit from Open settings." << std::endl;
                            break;
                        }

                        fin.open(fpath);

                        fin >> this->world_;

                        fin.close();

                        std::cout << "Exit from Open settings." << std::endl;

                        break;
                    }

                    default:
                        break;

                }

                break;
            }

            case sf::Event::Resized:
            {
                sf::Vector2i windowPosition = this->window_.getPosition();

                this->world_.resize(this->window_.getSize().x / this->scale_, this->window_.getSize().y / this->scale_);

                this->window_.create(sf::VideoMode(this->window_.getSize().x, this->window_.getSize().y), "Game of life");

                this->window_.setPosition(windowPosition);

                break;
            }

            default:
                break;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
    {
        mouse_position = this->window_.mapPixelToCoords(sf::Mouse::getPosition(this->window_));
        mouse_position.x /= static_cast<float>(this->scale_);
        mouse_position.y /= static_cast<float>(this->scale_);

        if (0 <= mouse_position.y && mouse_position.y < static_cast<float>(this->world_.height()) &&
            0 <= mouse_position.x && mouse_position.x < static_cast<float>(this->world_.width()))
        {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {

                this->world_.at(static_cast<unsigned >(mouse_position.y),
                                static_cast<unsigned >(mouse_position.x)) = true;

            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {

                this->world_.at(static_cast<unsigned >(mouse_position.y),
                                static_cast<unsigned >(mouse_position.x)) = false;
            }
        }
    }

    if (!this->isStop_ && this->timeFromLastStep_.getElapsedTime() > sf::milliseconds(this->stepInterval_))
    {
        this->world_.makeStep();

        this->timeFromLastStep_.restart();
    }
}
