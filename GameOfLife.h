#ifndef GAMEOFLIFE_GAMEOFLIFE_H
#define GAMEOFLIFE_GAMEOFLIFE_H


#include <exception>
#include <ctime>
#include <fstream>
#include <SFML/Graphics.hpp>

class GameOfLife
{
    class World
    {
    public:
        class OutOfMap : public std::exception
        {
            const char *what() const noexcept override;
        };

    public:
        class Rules
        {
        public:
            class OutOfRules : public std::exception
            {
            public:
                const char *what() const noexcept override;
            };

        public:
            Rules() = default;

            Rules(const GameOfLife::World::Rules &rules);


            GameOfLife::World::Rules & operator=(const GameOfLife::World::Rules &rules);


            void b_set(unsigned index);

            void b_reset(unsigned index);

            const bool &b_test(unsigned index) const;

            bool &b(unsigned index);


            void s_set(unsigned index);

            void s_reset(unsigned index);

            const bool &s_test(unsigned index) const;

            bool &s(unsigned index);

        private:
            bool b_[9]{false, false, false, false, false, false, false, false, false};
            bool s_[9]{false, false, false, false, false, false, false, false, false};

        };

    public:
        World();

        World(unsigned width, unsigned height);

        World(const World &world);


        World &operator=(const World &world);


        void resize(unsigned width, unsigned height);


        const unsigned &width() const;

        const unsigned &height() const;

        World::Rules &rules();

        bool &at(unsigned row, unsigned column);


        void clear();

        void fill_rand(unsigned seed = std::time(nullptr));

        void inversion();


        void makeStep();


        friend std::ostream &operator<<(std::ostream &os, const GameOfLife::World &world);

        friend std::istream &operator>>(std::istream &is, GameOfLife::World &world);


        ~World();

    private:
        unsigned width_, height_;
        bool **grid_;
        bool **bufferGrid_;

        World::Rules rules_;

    };

public:
    GameOfLife(unsigned width = 100, unsigned height = 100, unsigned scale = 10);


    GameOfLife::World::Rules &rules();


    void run(bool grid = false);

    friend std::ostream &operator<<(std::ostream &os, const GameOfLife::World &world);

    friend std::istream &operator>>(std::istream &is, GameOfLife::World &world);

private:
    unsigned scale_;

    World world_;

    sf::RenderWindow window_;
    sf::Clock timeFromLastStep_;
    unsigned stepInterval_;
    bool isStop_;

    void handleEvent_(sf::Event &event);

};


#endif //GAMEOFLIFE_GAMEOFLIFE_H
