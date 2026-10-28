#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// Incorrectly putting every behavior in one Robot base class would force
// unrelated robots to implement methods they do not support. The abstract
// interfaces below separate only the capabilities a robot actually owns.

class Robot {
protected:
    string name;

public:
    explicit Robot(string name) : name(move(name)) {}
    virtual ~Robot() = default;

    virtual void projection() const {
        cout << name << " robot";
    }
};

class Talkable {
public:
    virtual ~Talkable() = default;
    virtual void talk() const = 0;
};

class Walkable {
public:
    virtual ~Walkable() = default;
    virtual void walk() const = 0;
};

class Flyable {
public:
    virtual ~Flyable() = default;
    virtual void fly() const = 0;
};

// Concrete behavior families. They can be reused by every robot that has
// the matching capability; no giant if/else chain is needed in main().
class FriendlyTalkBehavior : public Talkable {
public:
    void talk() const override {
        cout << "Says: Hello! How can I help?" << '\n';
    }
};

class AssistiveWalkBehavior : public Walkable {
public:
    void walk() const override {
        cout << "Walks carefully on two legs" << '\n';
    }
};

class BasicFlightBehavior : public Flyable {
public:
    void fly() const override {
        cout << "Flies using propellers" << '\n';
    }
};

// Walkable-only robot.
class ServiceRobot : public Robot, public Walkable {
public:
    explicit ServiceRobot(string name) : Robot(move(name)) {}

    void walk() const override {
        cout << name << " -> ";
        AssistiveWalkBehavior{}.walk();
    }
};

// Talkable-only robot.
class ReceptionRobot : public Robot, public Talkable {
public:
    explicit ReceptionRobot(string name) : Robot(move(name)) {}

    void talk() const override {
        cout << name << " -> ";
        FriendlyTalkBehavior{}.talk();
    }
};

// A flyable robot. Its flight behavior is independent of the robot type.
class DroneRobot : public Robot, public Flyable {
public:
    explicit DroneRobot(string name) : Robot(move(name)) {}

    void fly() const override {
        cout << name << " -> ";
        BasicFlightBehavior{}.fly();
    }
};

// A robot can implement more than one capability without inheriting every
// behavior into a common base class.
class HumanoidRobot : public Robot, public Talkable, public Walkable, public Flyable {
public:
    explicit HumanoidRobot(string name) : Robot(move(name)) {}

    void talk() const override {
        cout << name << " -> ";
        FriendlyTalkBehavior{}.talk();
    }

    void walk() const override {
        cout << name << " -> ";
        AssistiveWalkBehavior{}.walk();
    }

    void fly() const override {
        cout << name << " -> ";
        BasicFlightBehavior{}.fly();
    }
};

void demonstrate(const Robot& robot) {
    robot.projection();
    cout << '\n';
}

int main() {
    ReceptionRobot receptionist("R1");
    ServiceRobot service("S1");
    DroneRobot drone("D1");
    HumanoidRobot humanoid("H1");

    cout << "Strategy Design Principle Demo\n";
    cout << "------------------------------\n";

    vector<Robot*> robots{&receptionist, &service, &drone, &humanoid};
    for (const Robot* robot : robots) {
        demonstrate(*robot);
    }

    receptionist.talk();
    service.walk();
    drone.fly();

    humanoid.talk();
    humanoid.walk();
    humanoid.fly();

    return 0;
}
