#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_set>
#include <random>
#include <string>

static  const   unsigned int DAYS = 366;
static          size_t SAMPLES = 1'000'0;

template<typename Int = unsigned int>
class Dice {
public:
    explicit Dice(const Int sides = 6)
        : m_sides{sides}
        , m_gen{ std::random_device{}() }
        , m_distribution{1, sides}
    {}

    Dice(const Dice &) = delete;
    Dice(Dice &&) = delete;

    Int roll()
    {
        if (m_sides == 0) return 0; // Broken dice
        if (m_sides == 1) return 1; // Well, you asked for this

        return m_distribution(m_gen);
    }

private:
    const Int m_sides;
    std::mt19937 m_gen;
    std::uniform_int_distribution<Int> m_distribution;
};

using DiceType = unsigned int;
static Dice<DiceType> DICE{DAYS};

template<typename Real = float>
Real pSame(const size_t eventsCount)
{
    unsigned int birthdayRepeats = 0;

    std::unordered_set<DiceType> events;
    for (size_t n = 0; n < SAMPLES; n++) {

        for (size_t e = 0; e < eventsCount; e++) {
            auto event = DICE.roll();

            if ( events.count(event) ) {
                birthdayRepeats++;
                break;
            }

            events.insert(event);
        }

        events.clear();
    }

    return Real(birthdayRepeats) / SAMPLES;
}

using Probabilty = float;
using Probabilities = std::vector<Probabilty>;

Probabilities getPs()
{
    Probabilities result;
    result.reserve(DAYS + 1);

    for (size_t i = 0; i <= DAYS; i++) {
        result.push_back( pSame< decltype(result)::value_type >(i) );
        std::cout << "Persons: " << i << std::endl;
    }

    return result;
}

void drawGraph(const Probabilities &ps, const size_t columns = 70)
{
    const float step = float( ps.size() ) / columns;
    const size_t width = 7;
    for (size_t row = 0; row <= 10; row++) {
        using Real = Probabilities::value_type;
        Real threshold = float(10 - row) / 10.0f;
        std::cout << std::fixed << std::setprecision(1) << threshold << '|';

        for (size_t column = 0; column < columns; column++) {
            size_t i = step * column;
            if ( ps.at(i) >= threshold ) {
                if (column % width) {
                    std::cout << '*';
                } else {
                    std::cout << '|';
                }

            } else {
                std::cout << ' ';
            }
        }

        std::cout << '\n';
    }

    std::cout << "   0^";
    for (size_t column = width; column < columns; column += width) {
        std::cout << std::setw(width - 1) << int(column * step) << '^';
    }
    std::cout << '\n';
}

int main(int argc, char *argv[]) {
    bool graph = false;
    size_t width = 70;

    if (argc > 1) {
        try {
            for (int arg = 1; arg < argc; arg++) {
                if ( std::string{ argv[arg] } == "--graph" ) {
                    graph = true;
                }

                if ( std::string{ argv[arg] } == "--width" ) {
                    int i = std::stoi( argv[arg + 1] );

                    if (i < 0) {
                        std::cout << "--width should be >= 0." << '\n';
                        return 0;
                    }

                    width = i;
                }

                if ( std::string{ argv[arg] } == "--samples" ) {
                    int i = std::stoi( argv[arg + 1] );

                    if (i < 0) {
                        std::cout << "--samples should be >= 0." << '\n';
                        return 0;
                    }

                    SAMPLES = i;
                }
            }
        }
        catch (std::invalid_argument const &e)
        {
            std::cout << "Bad input: std::invalid_argument thrown." << '\n';
            return 0;
        }
        catch (std::out_of_range const &e)
        {
            std::cout << "Integer overflow: std::out_of_range thrown." << '\n';
            return 0;
        }
    }

    std::cout << "Each test will collect " << SAMPLES << " samples." << '\n';

    const auto &ps = getPs();

    if(!graph) {
        for (size_t i = 0; i < ps.size(); i++) {
            std::cout << "For " << i << " events the probability is " << std::fixed << std::setprecision(3) << ps.at(i) << '\n';
        }
    } else {
        drawGraph(ps, width);
    }
    return 0;
}
