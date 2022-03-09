/*
 * THIS IS AN INTERACTIVE EXAMPLE
 * A dynamic set of trains needs to be maintained, where new trains are added and old trains removed randomly
 * Each train has an ID, name and pickup time
 * Additionally, queries maybe asked at any time where all trains whose pickup times are withing a given interval
 * need to be reported
*/

#include <iostream>
#include <string>

#include <limits>

#include "AgAVLTree.h"

/**
 * @brief                   Structure to store time in 24 hour format
 *
 */
struct train_time {
    int     mHours;
    int     mMinutes;

    /**
     * @brief               Given an integer which contains the time in 24 hour format, converts it to a train_time instance
     *
     * @param pTime         intger containing time in 24 hour formazt
     * @return train_time   object containing ptime converted to train_time object
     */
    static train_time
    from_24_hours (const int pTime)
    {
        int     hours;
        int     minutes;

        minutes     = pTime % 100;
        hours       = pTime / 100;

        return {hours, minutes};
    }

    /**
     * @brief               Given a train_time object, convert it to an integer suitable for printing to the console
     *
     * @param pTime         train_time instance
     * @return int          integer containing pTime converted to 24 hour format
     */
    static int
    to_24_hours (const train_time &pTime) {
        return (pTime.mHours * 100) + (pTime.mMinutes);
    }

    /**
     * @brief               Compares two train_time objects
     *
     * @param pA            First train_time to compare with
     * @param pB            Second train_time to compare to
     * @return true         If pA comes before pB on the clock
     * @return false        If PA does not come before pB on the clock
     */
    friend bool
    operator< (const train_time &pA, const train_time &pB)
    {
        // if hour is same, compare minutes
        if (pA.mHours == pB.mHours) {
            return pA.mMinutes < pB.mMinutes;
        }

        // whichever has a smaller value of hour
        return pA.mHours < pB.mHours;
    }

    /**
     * @brief               Checks for equality for two train_time objects
     *
     * @param pA            First train_time to compare with
     * @param pB            Second train_time to compare to
     * @return true         If pA and pB appear at the same time on the clock
     * @return false        If pA and pB do not appear at the same time on the clock
     */
    friend bool
    operator== (const train_time &pA, const train_time &pB)
    {
        // for equality, both times must compare exactly the same
        return (pA.mHours == pB.mHours) && (pA.mMinutes == pB.mMinutes);
    }
};

/**
 * @brief               Structure to store simplified information about a train (ID, name and pickup time)
 *
 */
struct train {

    int         mId;
    std::string mName;
    train_time  mPickUp;

    /**
     * @brief           Compares two trqins based on their pickup time (breaking ties by their IDs)
     *
     * @param pA        First train to compare with
     * @param pB        Second train to compare to
     * @return true     If the pickup time of pA is less than that of pB (break ties by ID, which must be unique)
     * @return false    If the pickup time of pA is greater than pB (or if they are equal and pB's ID is less)
     */
    friend bool
    operator< (const train &pA, const train &pB)
    {
        if (pA.mPickUp == pB.mPickUp) {
            return pA.mId < pB.mId;
        }
        return pA.mPickUp < pB.mPickUp;
    }

    /**
     * @brief           Checks for equality of two trains based on iD
     *
     * @param pA        First train to compare with
     * @param pB        Second train to compare to
     * @return true     if id of pA is strictly equal to the id of pB
     * @return false    if id of pA is not strictly equal to the id of pB
     */
    friend bool
    operator== (const train &pA, const train &pB)
    {
        return pA.mId == pB.mId;
    }
};

/**
 * @brief               Compares two trains based on their ID
 *
 * @param pA            First train to compare with
 * @param pb            Second train to compare to
 * @return true         if id of pA is strictly less than id of pB
 * @return false        if id of pA is not strictly less than id of pB
 */
bool
train_id_lt (const train &pA, const train &pB)
{
    return pA.mId < pB.mId;
}

/**
 * @brief               Checks for equality fo two trains based on ID
 *
 * @param pA            First train to compare with
 * @param pb            Second train to compare to
 * @return true         if id of pA is strictly equal to the id of pB
 * @return false        if id of pA is not strictly equal to the id of pB
 */
bool
train_id_eq (const train &a, const train &b)
{
    return a.mId == b.mId;
}

/**
 * @brief               Prints a list of options for the user to choose from
 *
 */
void print_options () {
    std::cout << "1.\tInsert a new train\n";
    std::cout << "2.\tRemove a train\n";
    std::cout << "3.\tFind trains (between a certain timing)\n";
    std::cout << "4.\tShow all trains\n";
    std::cout << "5.\tExit\n";
}

int
main (void)
{

    // create two trees to store train, one sorted on time and another sorted based on ID
    AgAVLTree<train>                            trainTree;
    AgAVLTree<train, train_id_lt, train_id_eq>  idTree;

    print_options ();

    while (1) {
        int                 opt;
        std::cout << ">> ";
        std::cin >> opt;

        // if an invalid option was selected, re-print the options and ask the user to re-select an option
        if (opt < 1 || opt > 5) {

            std::cout << "\nPlease enter one of the following options\n";

            print_options ();

            continue;
        }

        std::cout << '\n';

        // Insert a new train
        if (opt == 1) {

            // receive the ID, name and pickup time of the train as input from the user
            int             id;
            std::string     name;
            int             pickup;

            std::cout << "Id:\t\t";
            std::cin >> id;

            std::cout << "Name:\t\t";
            std:: cin >> name;

            std::cout << "Pickup time (in 24 hour format):\t";
            std::cin >> pickup;

            // if a train with the given ID already exists, do not move further
            if (!idTree.insert ({id, name, train_time::from_24_hours (pickup)})) {
                std::cout << "Train with matching ID already exists\n";
                continue;
            }

            // if a train with the given ID does not exist, insert into the tree
            trainTree.insert ({id, name, train_time::from_24_hours (pickup)});
        }

        // Erase an old train
        else if(opt == 2) {

            // receive the ID of the train as input from the user
            int             id;

            std::cout << "Id:\t\t";
            std::cin >> id;

            // try to find if a train with this ID exists, also get its remaining details (name and pickup time)
            auto            it  = idTree.find ({id, "", {-1, -1}});

            // if such a train does not exist, do not move further
            if (it == idTree.end ()) {
                std::cout << "No train with matching ID could be found\n";
                continue;
            }

            // erase the train from both trees
            idTree.erase (*it);
            trainTree.erase (*it);
        }

        // print all trains withing a time interval
        else if (opt == 3) {

            // recieve the start and end of the interval as input from the user (in 24 hour format)
            int     start;
            int     end;

            AgAVLTree<train>::iterator  lo;
            AgAVLTree<train>::iterator  hi;

            std::cout << "Interval beginning:\t";
            std::cin >> start;

            std::cout << "Interval ending:\t";
            std::cin >> end;

            // the first train in the segment will be the earliest train whose pickup time is greater than or equal to the interval start
            lo  = trainTree.first_greater_equals ({-1, "", train_time::from_24_hours (start)});

            // the train after the last one is the smallest train whose pickup time is strictly greater than the interval end
            hi  = trainTree.first_greater_strict ({std::numeric_limits<int>::max (), "", train_time::from_24_hours (end)});

            // if the lower bound of the interval is more than the latest train's pickup, do not move further
            if (lo == trainTree.end ()) {
                continue;
            }

            // iterate through the segment [lo, hi) and print all trains
            for (auto it = lo; it != hi; ++it) {
                auto &[id, name, pickup]    = *it;

                std::cout << '#' << id << '\t' << name << "\tleaves at " << train_time::to_24_hours (pickup) << " hours\n";
            }
        }

        // print all trains in set
        else if (opt == 4) {

            // iterate through the entire set of trains and print them
            for (auto &[id, name, pickup] : idTree) {
                std::cout << '#' << id << '\t' << name << "\tleaves at " << train_time::to_24_hours (pickup) << " hours\n";
            }
        }

        // exit
        else if (opt == 5) {
            break;
        }

        std::cout << '\n';
    }

    return 0;
}
