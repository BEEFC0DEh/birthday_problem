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

int main(int argc, char *argv[]) {
    if (argc > 1) {
        try {
            int i = std::stoi( argv[1] );

            if (i < 0) {
                std::cout << "SAMPLES should be >= 0." << '\n';
                return 0;
            }

            SAMPLES = i;
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

    const auto &ps = getPs();
    for (size_t i = 0; i < ps.size(); i++) {
        std::cout << "For " << i << " events the probability is " << std::fixed << std::setprecision(3) << ps.at(i) << '\n';
    }
    return 0;
}
