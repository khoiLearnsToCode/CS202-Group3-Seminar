#include <iostream>
#include <vector>
#include <string>

// Forward declarations
class Student;
class Teacher;
class Equipment;
class Class;
class Visitor;

// <<interface>> SchoolEntity
class SchoolEntity {
public:
    virtual void updateStatus(const std::string& status) = 0;
    virtual std::string getDetail(const std::string& context) const = 0;
    virtual void accept(Visitor* v) = 0;
    virtual ~SchoolEntity() = default;
};

// Each concrete element class must implement the SchoolEntity interface
// and the `accept` method.

class Student : public SchoolEntity {
private:
    std::string studentId;
    std::string name;
    std::string status;

public:
    Student(std::string id, std::string n) : studentId(id), name(n), status("Active") {}

    void updateStatus(const std::string& newStatus) override {
        status = newStatus;
        std::cout << "Student " << name << " status updated to: " << status << std::endl;
    }

    std::string getDetail(const std::string& context) const override {
        if (context == "id") return studentId;
        if (context == "name") return name;
        return "Student: ID=" + studentId + ", Name=" + name + ", Status=" + status;
    }

    void accept(Visitor* v) override;
};

class Teacher : public SchoolEntity {
private:
    std::string employeeId;
    std::string subject;
    std::string status;

public:
    Teacher(std::string id, std::string sub) : employeeId(id), subject(sub), status("Active") {}

    void updateStatus(const std::string& newStatus) override {
        status = newStatus;
        std::cout << "Teacher " << employeeId << " status updated to: " << status << std::endl;
    }

    std::string getDetail(const std::string& context) const override {
        if (context == "id") return employeeId;
        if (context == "subject") return subject;
        return "Teacher: ID=" + employeeId + ", Subject=" + subject + ", Status=" + status;
    }

    void accept(Visitor* v) override;
};

class Equipment : public SchoolEntity {
private:
    std::string itemId;
    std::string description;
    std::string status;

public:
    Equipment(std::string id, std::string desc) : itemId(id), description(desc), status("In Use") {}

    void updateStatus(const std::string& newStatus) override {
        status = newStatus;
        std::cout << "Equipment " << itemId << " status updated to: " << status << std::endl;
    }

    std::string getDetail(const std::string& context) const override {
        if (context == "id") return itemId;
        if (context == "description") return description;
        return "Equipment: ID=" + itemId + ", Description=" + description + ", Status=" + status;
    }

    void accept(Visitor* v) override;
};

class Class : public SchoolEntity {
private:
    std::string name;
    std::vector<SchoolEntity*> members;
    std::string status;

public:
    Class(std::string n) : name(n), status("Active") {}

    void addMember(SchoolEntity* member) {
        members.push_back(member);
    }

    std::string getName() const {
        return name;
    }

    const std::vector<SchoolEntity*>& getMembers() const {
        return members;
    }

    void updateStatus(const std::string& newStatus) override {
        status = newStatus;
        std::cout << "Class " << name << " status updated to: " << status << std::endl;
    }

    std::string getDetail(const std::string& context) const override {
        if (context == "name") return name;
        std::string details = "Class: Name=" + name + ", Status=" + status + ", Members=[";
        for (size_t i = 0; i < members.size(); ++i) {
            details += members[i]->getDetail("name");
            if (i < members.size() - 1) {
                details += ", ";
            }
        }
        details += "]";
        return details;
    }

    void accept(Visitor* v) override;
};

// <<interface>> Visitor
class Visitor {
public:
    virtual void visitClass(Class* c) = 0;
    virtual void visitStudent(Student* s) = 0;
    virtual void visitTeacher(Teacher* t) = 0;
    virtual void visitEquipment(Equipment* e) = 0;
    virtual ~Visitor() = default;
};

class XMLExportVisitor : public Visitor {
public:
    void visitClass(Class* c) override {
        std::cout << "<class name='" << c->getName() << "'>" << std::endl;
        for (SchoolEntity* member : c->getMembers()) {
            member->accept(this);
        }
        std::cout << "</class>" << std::endl;
    }

    void visitStudent(Student* s) override {
        std::cout << "  <student id='" << s->getDetail("id") << "' name='" << s->getDetail("name") << "'/>" << std::endl;
    }

    void visitTeacher(Teacher* t) override {
        std::cout << "  <teacher id='" << t->getDetail("id") << "' subject='" << t->getDetail("subject") << "'/>" << std::endl;
    }

    void visitEquipment(Equipment* e) override {
        std::cout << "  <equipment id='" << e->getDetail("id") << "' description='" << e->getDetail("description") << "'/>" << std::endl;
    }
};

// Implement accept methods
void Student::accept(Visitor* v) {
    v->visitStudent(this);
}

void Teacher::accept(Visitor* v) {
    v->visitTeacher(this);
}

void Equipment::accept(Visitor* v) {
    v->visitEquipment(this);
}

void Class::accept(Visitor* v) {
    v->visitClass(this);
}

class Application {
private:
    std::vector<SchoolEntity*> entities;

public:
    void addEntity(SchoolEntity* entity) {
        entities.push_back(entity);
    }

    void exportToXML() {
        XMLExportVisitor exporter;
        std::cout << "<school>" << std::endl;
        for (SchoolEntity* entity : entities) {
            entity->accept(&exporter);
        }
        std::cout << "</school>" << std::endl;
    }
};

int main() {
    Application schoolApp;

    Class* scienceClass = new Class("Science 101");
    Student* student1 = new Student("S1001", "Alice");
    Student* student2 = new Student("S1002", "Bob");
    Teacher* teacher1 = new Teacher("T2001", "Physics");
    Equipment* microscope = new Equipment("E3001", "High-power microscope");

    scienceClass->addMember(student1);
    scienceClass->addMember(teacher1);
    scienceClass->addMember(microscope);

    schoolApp.addEntity(scienceClass);
    schoolApp.addEntity(student2); // Adding a standalone student

    std::cout << "--- Exporting School Data to XML ---" << std::endl;
    schoolApp.exportToXML();

    delete scienceClass;
    delete student1;
    delete student2;
    delete teacher1;
    delete microscope;

    return 0;
}