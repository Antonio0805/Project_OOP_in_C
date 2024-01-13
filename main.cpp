#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
#include <ctime>

using namespace std;

map<int, float> numberInputValues; // Harta globală pentru stocarea valorilor NUMBER INPUT
map<int, string> textInputValues;  // Pentru TEXT INPUT
map<int, string> csvInputValues;   // Pentru CSV INPUT
map<int, string> textInputDescriptions;  // Pentru descrierile TEXT INPUT
map<int, string> csvInputDescriptions;  // Pentru descrierile CSV INPUT
map<int, pair<string, string>> textFileInputInfo;
map<int, pair<string, string>> csvFileInputInfo;
map<int, int> stepSkipCount; // Harta pentru numărarea omisiunilor fiecărui pas



class Step //clasa pt un pas
{
public:
    virtual void execute() = 0; //metoda pt executarea unui pas
    virtual void DisplayStepInfo() const = 0;
    virtual void DetailsToFile(ofstream& file) const = 0; //la fiecare pas, scrie detaliile in fisier
    virtual ~Step() = default;
};



class TITLEstep: public Step //clasa pt pasul TITLE
{
private:
    std::string title;
    std::string subtitle;

public:
    TITLEstep(const string &t, const string &st)
    {
        title = t;
        subtitle = st;
    }

    void execute() override
    {
        // implementați logica execuției pasului TITLE
    }

    void DisplayStepInfo() const override
    {
        cout << "TITLE Step: " << "\n";
        cout << "Title: " << title << "\n";
        cout << "Subtitle: " << subtitle << "\n";
    }

    void DetailsToFile(ofstream &file) const override
    {
        file << "TITLE Step: " << "\n";
        file << "Title: " << title << "\n";
        file << "Subtitle: " << subtitle << "\n";
    }
};




class TEXTstep: public Step //clasa pt pasul TEXT
{
private:
    string title;
    string copy;

public:
    TEXTstep(const string& t, const string& c)
    {
        title = t;
        copy = c;
    }

    void execute() override
    {
        // implementați logica execuției pasului TITLE
    }


    void DisplayStepInfo() const override
    {
        cout << "TEXT Step: " << "\n";
        cout << "Title: " << title <<"\n";
        cout << "Copy: "<< copy << "\n";

    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "TEXT Step: " << "\n";
        file << "Title: " << title << "\n";
        file << "Copy: " << copy << "\n";
    }

};


class TEXTINPUTstep: public Step
{
private:
    string description; // Descrierea pasului
    string textinput; // Textul introdus de utilizator
    int id; // Identificator unic pentru acest pas
    static int idCounter; // Contor static pentru generarea ID-urilor unice

public:
    // Constructorul care acceptă doar descrierea
    TEXTINPUTstep(const string& d) : description(d), textinput("")
    {
        id = ++idCounter; // Incrementați contorul și asignați ID-ul
    }

    // Metoda execute modificată pentru a cere introducerea textului
    void execute() override
    {
        if (textInputValues.find(id) == textInputValues.end())
        {
            cout << "Enter text for \"" << description << "\": ";
            getline(cin, textinput);
            textInputValues[id] = textinput; // Stocăm textul introdus în map-ul global
            cout << "Textul introdus \"" << textinput << "\" a fost stocat cu ID-ul " << id << endl;
        }
        else
        {
            // Dacă textul a fost deja introdus, îl afișăm
            cout << "Textul pentru \"" << description << "\" este deja introdus: " << textInputValues[id] << endl;
            // Dăm posibilitatea de a actualiza textul
            cout << "Doriti sa actualizati textul? (y/n): ";
            char response;
            cin >> response;
            if (response == 'y' || response == 'Y')
            {
                cin.ignore(); // Curățăm buffer-ul
                cout << "Enter new text for \"" << description << "\": ";
                getline(cin, textinput);
                textInputValues[id] = textinput; // Actualizăm textul introdus în map-ul global
                cout << "Noul text introdus \"" << textinput << "\" a fost stocat cu ID-ul " << id << endl;
            }
        }
    }
    // Metoda pentru afișarea informațiilor pasului
    void DisplayStepInfo() const override
    {
        string  textINPUT;
        cout << "TEXT INPUT Step: " << "\n";
        cout << "Description: " << description << "\n";
        // Verificăm dacă există o valoare în map pentru ID-ul curent
        if (textInputValues.find(id) != textInputValues.end())
        {
            cout << "Text Input: " << textInputValues[id] << "\n";
            getline(std::cin,textINPUT);
        }
        else
        {
            cout << "Text Input: "  << "\n";
            getline(cin,textINPUT);
        }
        cout << "ID: " << id << "\n";
    }

    // Metoda pentru scrierea detaliilor pasului într-un fișier
    void DetailsToFile(ofstream& file) const override
    {
        file << "TEXT INPUT Step: " << "\n";
        file << "Description: " << description << "\n";
        // Verificăm dacă există o valoare în map pentru ID-ul curent
        if (textInputValues.find(id) != textInputValues.end())
        {
            file << "Text Input: " << textInputValues[id] << "\n";
        }
        else
        {
            file << "Text Input: " << "[Textul va fi introdus]" << "\n";
        }
        file << "ID: " << id << "\n";
    }
};


int TEXTINPUTstep::idCounter = 0; // Inițializare contor ID


class NUMBERINPUTstep: public Step
{
private:
    string description;
    float numberinput;
    int id; // Identificator unic pentru acest pas
    static int idCounter; // Contor static pentru generarea ID-urilor unice

public:
    NUMBERINPUTstep(const string& d, float ni)
        : description(d), numberinput(ni)
    {
        id = ++idCounter; // Incrementați contorul și asignați ID-ul
    }

    void execute() override
    {
        numberInputValues[id] = numberinput;
        cout << "Valoarea introdusa (" << numberinput << ") a fost stocata cu ID-ul " << id << endl;
    }

    void DisplayStepInfo() const override
    {
        cout << "NUMBER INPUT Step: " << "\n";
        cout << "Description: " << description << "\n";
        cout << "Number Input: " << numberinput << "\n";
        cout << "ID: " << id << "\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "NUMBER INPUT Step: " << "\n";
        file << "Description: " << description << "\n";
        file << "Number Input: " << numberinput << "\n";
        file << "ID: " << id << "\n";
    }

    static void resetIdCounter()
    {
        idCounter = 0; // Resetare la 0 la începutul fiecărui flux
    }
};


int NUMBERINPUTstep::idCounter = 0; // Inițializare contor ID


class CALCULUSstep: public Step
{
private:
    vector<int> stepIndices; // Indicii pașilor de tip NUMBER INPUT
    vector<string> operations; // Lista de operații modificate pentru a include min și max

public:
    CALCULUSstep() {}

    void addStep(int index)
    {
        stepIndices.push_back(index); // Adaugă un indice de pas
    }

    void addOperation(string op)
    {
        operations.push_back(op); // Adaugă un simbol de operație sau cuvinte cheie min/max
    }

    void execute() override
    {
        if (stepIndices.empty() || stepIndices.size() != operations.size() + 1)
        {
            cout << "Numar incorect de pasi sau operatii." << endl;
            return;
        }

        float result = numberInputValues[stepIndices[0]];

        for (size_t i = 0; i < operations.size(); ++i)
        {
            float nextValue = numberInputValues[stepIndices[i + 1]];
            if (operations[i] == "+")
            {
                result += nextValue;
            }
            else if (operations[i] == "-")
            {
                result -= nextValue;
            }
            else if (operations[i] == "*")
            {
                result *= nextValue;
            }
            else if (operations[i] == "/")
            {
                if (nextValue == 0)
                {
                    cout << "Eroare: împartire la zero." << endl;
                    return;
                }
                result /= nextValue;
            }
            else if (operations[i] == "min")
            {
                result = min(result, nextValue);
            }
            else if (operations[i] == "max")
            {
                result = max(result, nextValue);
            }
            else
            {
                cout << "Operatie necunoscuta: " << operations[i] << std::endl;
                return;
            }
        }

        cout << "Rezultatul calculului: " << result << endl;
    }


    void DisplayStepInfo() const override
    {
        cout << "CALCULUS Step:\n";
        cout << "Pasii implicati: ";
        for (const auto& index : stepIndices)
        {
            cout << index << " ";
        }
        cout << "\nOperatii: ";
        for (const auto& op : operations)
        {
            cout << op << " ";
        }
        cout << endl;
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "CALCULUS Step:\n";
        file << "Pasii implicati: ";
        for (const auto& index : stepIndices)
        {
            file << index << " ";
        }
        file << "\nOperatii: ";
        for (const auto& op : operations)
        {
            file << op << " ";
        }
        file << "\n";
    }
};



class Display : public Step
{
private:
    int stepIndex;
    string stepType;

public:
    Display(int index, const string& type) : stepIndex(index), stepType(type) {}

    void execute() override
    {
        // Logica pentru executarea pasului Display
        // Afișează informațiile despre fișier în funcție de tipul pasului
        displayFileInfo();
    }
    void displayImmediateInfo()
    {
        // Logica similară cu cea din DisplayStepInfo

        if (stepType == "TEXT FILE INPUT" && textFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = textFileInputInfo.at(stepIndex);
            cout << "Nume fisier: " << fileInfo.first << "\n";
            cout << "Descriere fisier: " << fileInfo.second << "\n";
        }

        else if (stepType == "CSV FILE INPUT" && csvFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = csvFileInputInfo.at(stepIndex);
            cout << "Nume fisier: " << fileInfo.first << "\n";
            cout << "Descriere fisier: " << fileInfo.second << "\n";
        }

        else
        {
            cout << "Nu există informații de afișat pentru acest pas.\n";
        }
    }


    void DisplayStepInfo() const override
    {
        cout << "DISPLAY Step:\n";
        cout << "Step Index: " << stepIndex << "\n";
        cout << "Step Type: " << stepType << "\n";
        // Afișează informațiile despre fișier în consolă
        displayFileInfo();
    }



    void DetailsToFile(ofstream& file) const override
    {
        file << "DISPLAY Step:\n";
        file << "Step Index: " << stepIndex << "\n";
        file << "Step Type: " << stepType << "\n";
        // Scrie informațiile despre fișier într-un fișier
        fileFileInfo(file);
    }

private:
    void displayFileInfo() const
    {
        if (stepType == "TEXT FILE INPUT" && textFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = textFileInputInfo.at(stepIndex);
            cout << "Nume fisier: " << fileInfo.first << "\n";
            cout << "Descriere fisier: " << fileInfo.second << "\n";
        }
        else if (stepType == "CSV FILE INPUT" && csvFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = csvFileInputInfo.at(stepIndex);
            cout << "Nume fisier: " << fileInfo.first << "\n";
            cout << "Descriere fisier: " << fileInfo.second << "\n";
        }
    }

    void fileFileInfo(ofstream& file) const
    {
        if (stepType == "TEXT FILE INPUT" && textFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = textFileInputInfo.at(stepIndex);
            file << "Nume fisier: " << fileInfo.first << "\n";
            file << "Descriere fisier: " << fileInfo.second << "\n";
        }
        else if (stepType == "CSV FILE INPUT" && csvFileInputInfo.count(stepIndex))
        {
            const auto& fileInfo = csvFileInputInfo.at(stepIndex);
            file << "Nume fisier: " << fileInfo.first << "\n";
            file << "Descriere fisier: " << fileInfo.second << "\n";
        }
    }
};



class TEXTFILEINPUTstep: public Step
{
private:
    string description, file_name;
    int id;
    static int idCounter;

public:
    TEXTFILEINPUTstep(const string& d, const string &fn) : description(d), file_name(fn)
    {
        id = idCounter++; // Incrementează ID-ul la crearea fiecărui pas
    }

    void execute() override
    {
        // Adaugă extensia ".txt" dacă lipsește
        if (file_name.size() < 4 || file_name.substr(file_name.size() - 4) != ".txt")
        {
            file_name += ".txt";
        }

        ifstream file(file_name);
        string content;
        if (file.is_open())
        {
            content.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
        }
        else
        {
            content = "File not found or not accessible";
        }
        textInputValues[id] = content; // Stocare conținut în map
        cout << "Fisierul \"" << file_name << "\" a fost salvat cu step index " << id << " si este de tipul TEXT INPUT.\n";
        textInputDescriptions[id] = description;
        textFileInputInfo[id] = make_pair(file_name, description);
    }

    void DisplayStepInfo() const override
    {
        cout << "TEXT FILE INPUT Step:\n";
        cout << "Description: " << description << "\n";
        cout << "File name: " << file_name << "\n";
        cout << "ID: " << id << "\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "TEXT FILE INPUT Step:\n";
        file << "Description: " << description << "\n";
        file << "File name: " << file_name << "\n";
        file << "ID: " << id << "\n";
    }

    static int getNextId()
    {
        return idCounter++; // Returnează și incrementează ID-ul
    }
};



int TEXTFILEINPUTstep::idCounter = 1; // Inițializare contor ID



class CSVFILEINPUTstep: public Step
{
private:
    string description, file_name;
    int id;
    static int idCounter;

public:
    CSVFILEINPUTstep(const string& d, const string &fn) : description(d), file_name(fn)
    {
        id = idCounter++; // Asignează un ID unic fiecărui pas și incrementează contorul
    }
    void execute() override
    {
        // Adaugă extensia ".csv" dacă lipsește
        if (file_name.size() < 4 || file_name.substr(file_name.size() - 4) != ".csv")
        {
            file_name += ".csv";
        }

        ifstream file(file_name);
        string line, content;
        if (file.is_open())
        {
            while (getline(file, line))
            {
                content += line + "\n";
            }
            file.close();
        }
        else
        {
            content = "File not found or not accessible";
        }
        csvInputValues[id] = content; // Stocare conținut în map
        cout << "Fisierul \"" << file_name << "\" a fost salvat cu step index " << id << " si este de tipul CSV INPUT.\n";
        csvInputDescriptions[id] = description;
        csvFileInputInfo[id] = make_pair(file_name, description);
    }


    void DisplayStepInfo() const override
    {
        cout << "CSV FILE INPUT Step:\n";
        cout << "Description: " << description << "\n";
        cout << "File name: " << file_name << "\n";
        cout << "ID: " << id << "\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "CSV FILE INPUT Step:\n";
        file << "Description: " << description << "\n";
        file << "File name: " << file_name << "\n";
        file << "ID: " << id << "\n";
    }

    static int getNextId()
    {
        return idCounter++; // Returnează și incrementează ID-ul
    }
};



int CSVFILEINPUTstep::idCounter = 1; // Inițializare contor ID



class OUTPUTstep: public Step
{
private:
    int step; // Indexul pasului din care se vor extrage datele
    string file_name; // Numele fișierului de output
    string title; // Titlul pentru output
    string description; // Descrierea pentru output

public:
    OUTPUTstep(int s, const string& fn, const string& t, const string& d)
        : step(s), file_name(fn), title(t), description(d) {}

    void execute() override
    {
        ofstream outFile(file_name);
        if (!outFile.is_open())
        {
            cerr << "Unable to open file: " << file_name << endl;
            return;
        }

        outFile << "Title: " << title << "\n";
        outFile << "Description: " << description << "\n";

        // Extragerea și scrierea informațiilor din pasul specificat
        if (textInputValues.count(step))
        {
            outFile << "Text Input Data: " << textInputValues[step] << "\n";
        }
        else if (numberInputValues.count(step))
        {
            outFile << "Number Input Data: " << numberInputValues[step] << "\n";
        }
        // Aici poți adăuga logica pentru alte tipuri de pași, dacă este necesar

        outFile.close();
        cout << "Output file '" << file_name << "' has been created." << endl;
    }

    void DisplayStepInfo() const override
    {
        cout << "OUTPUT Step:\n";
        cout << "Source Step Index: " << step << "\n";
        cout << "Output File Name: " << file_name << "\n";
        cout << "Title: " << title << "\n";
        cout << "Description: " << description << "\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "OUTPUT Step:\n";
        file << "Source Step Index: " << step << "\n";
        file << "Output File Name: " << file_name << "\n";
        file << "Title: " << title << "\n";
        file << "Description: " << description << "\n";
    }
};




class ENDstep : public Step
{
public:
    ENDstep() {}

    void execute() override
    {
        cout << "This flow has ended.\n" << endl;
    }

    void DisplayStepInfo() const override
    {
        cout << "END Step: This step signifies the end of the flow.\n";
    }

    void DetailsToFile(ofstream& file) const override
    {
        file << "END Step: This step signifies the end of the flow.\n";
    }
};



class Flow
{
private:
    vector<Step*> steps; // structura de tip vector permite adăugarea unui număr nelimitat de pași -- cerinta 6
    string name;
    string creationTime; //stocheaza timestamp

    // Funcție utilitară pentru a obține timestamp-ul curent
    string getCurrentTimestamp()
    {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        // Formatează data și ora în format lizibil
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }

public:
    Flow(const string& flowName) : name(flowName)
    {
        creationTime = getCurrentTimestamp(); // Setează timestamp-ul la crearea obiectului
    }

    void addStep(Step* step)
    {
        steps.push_back(step);
    }

    string getFlowName() const
    {
        return name;
    }


    void executeFlow()
    {
        cout << "Executing Flow: " << name << "\n";
        cout << "Created at: " << creationTime << endl;
        int stepIndex = 0;

        for (auto& step : steps)
        {
            if (dynamic_cast<ENDstep*>(step) != nullptr)
            {
                cout << "Ending Flow early." << endl;
                break; // Încheie bucla for dacă întâlnește un pas ENDstep
            }

            char userChoice;
            cout << endl;
            cout << "Do you want to execute the next step? (y/n): ";
            cin >> userChoice;
            if (userChoice == 'y' || userChoice == 'Y')
            {
                step->DisplayStepInfo();
                step->execute();
            }
            else
            {
                cout << "Skipping step...\n";
                stepSkipCount[stepIndex]++; // Incrementați contorul de omisiuni pentru acest pas
            }
            stepIndex++;
        }
    }


    //afișarea tuturor fluxurilor cu indexul lor


    void displayFlowInfo() const
    {
        std::cout << "Flow: " << name << "\n";
        std::cout << "Created at: " << creationTime << "\n"; // Afișează timestamp-ul
        for (const auto& step : steps)
        {
            step->DisplayStepInfo();
        }
    }

    void saveFlowDetails(const string& filename) const
    {
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        file << "Flow Name: " << name << "\n";
        for (const auto& step : steps)
        {
            step->DetailsToFile(file);
        }
        file << "------\n";
        file.close();
    }

    ~Flow()
    {
        for (auto& step : steps)
        {
            delete step;
            step = nullptr;
        }
        steps.clear();
    }


    void displayStepSkipAnalytics() const
    {
        cout << "Analytics - Step Skip Counts:\n";
        for (const auto& pair : stepSkipCount)
        {
            cout << "Step " << pair.first << ": Skipped " << pair.second << " times\n";
        }
    }
};



void displayFlows(const vector<Flow>& allFlows)
{
    if (allFlows.empty())
    {
        cout << "No flows available." << endl;
        return;
    }
    cout << "Available Flows:" << endl;
    for (size_t i = 0; i < allFlows.size(); ++i)
    {
        cout << i << ": " << allFlows[i].getFlowName() << endl;
    }
}




// adăugam o funcție care afișează pașii disponibili și descrierile lor:
void displayAvailableSteps()
{
    cout<<endl;
    std::cout << "Available Steps:\n";
    std::cout << "1. TITLE Step: A step for adding a title and a subtitle.\n";
    std::cout << "2. TEXT Step: A step for adding a text block.\n";
    std::cout << "3. TEXT INPUT Step: A step for inputting text.\n";
    std::cout << "4. NUMBER INPUT Step: A step for inputting a numerical value.\n";
    std::cout << "5. CALCULUS Step: A step for performing calculations based on input numbers.\n";
    std::cout << "6. DISPLAY Step: A step for displaying information from previous steps.\n";
    std::cout << "7. TEXT FILE INPUT Step: A step for inputting text from a file.\n";
    std::cout << "8. CSV FILE INPUT Step: A step for inputting data from a CSV file.\n";
    std::cout << "9. OUTPUT Step: A step for generating a summary or output file based on the previous steps.\n";
    std::cout << "10. END Step: A step to signal the end of the flow.\n";
}

//Vom adăuga o nouă funcție care va permite crearea de pași specifici în funcție de opțiunile utilizatorului: (crearea unui now flow)
Step* createStep()      //permite utilizatorilor să adauge orice tip de pas în flux, fără restricții cu privire la tipul sau numărul de ori în care un anumit tip de pas poate fi adăugat-cerinta 7
{
    int choice;
    std::string title, subtitle, description, textinput, operation, file_name;
    float numberinput;
    int steps;
    cout<<endl;
    cout << "Select the type of step to add:\n";
    cout << "1. TITLE Step\n";
    cout << "2. TEXT Step\n";
    cout << "3. TEXT INPUT Step\n";
    cout << "4. NUMBER INPUT Step\n";
    cout << "5. CALCULUS Step\n";
    cout << "6. DISPLAY Step\n";
    cout << "7. TEXT FILE INPUT Step\n";
    cout << "8. CSV FILE INPUT Step\n";
    cout << "9. OUTPUT Step\n";
    cout << "10. END Step\n";
    cout << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
    case 1:
    {
        cout << "Enter title: "; //Ca utilizator, cand doresc sa adaug un pas de tip TITLE, trebuie sa adaug un titlu și un subtitlu -- cerinta 8
        getline(cin, title);
        cout << "Enter subtitle: ";
        getline(cin, subtitle);
        return new TITLEstep(title, subtitle);
    }


    case 2:
    {
        cout << "Enter title: ";
        getline(cin, title);
        cout << "Enter copy: ";
        getline(cin, subtitle);
        return new TEXTstep(title, subtitle);   // Cerinta 9
    }


    case 3:
    {
        cout << "Enter description: ";
        getline(cin, description);
        // Creați obiectul TEXTINPUTstep cu descrierea, fără a cere textul acum.
        // Textul va fi introdus când se execută pasul (în metoda execute).
        return new TEXTINPUTstep(description);
//Cerinta 10
    }



    case 4:
    {
        // Adăugare și execuție NUMBER INPUT Step
        cout << "Enter description: ";
        getline(cin, description);
        cout << "Enter number input: ";
        cin >> numberinput;
        cin.ignore();
        auto numberInputStep = new NUMBERINPUTstep(description, numberinput);  ////Cerinta 11 - la number input utilizatorul trebuie sa aleaga doar numere
        numberInputStep->execute(); // Executare imediată
        return numberInputStep;
    }




    case 5:   // Pentru pasul CALCULUS
    {
        auto calculusStep = new CALCULUSstep();
        int numSteps;
        cout << "Introduceti numarul de pasi de tip NUMBER INPUT pentru calcul: ";
        cin >> numSteps;
        cin.ignore(); // Pentru a curăța buffer-ul

        for (int i = 0; i < numSteps; ++i)
        {
            int stepIndex;
            string operation;
            cout << "Introduceti indicele pasului NUMBER INPUT " << i + 1 << ": ";
            cin >> stepIndex;
            calculusStep->addStep(stepIndex);

            if (i < numSteps - 1)
            {
                cout << "Introduceti simbolul operatiei (+, -, *, /, min, max) dupa pasul " << i + 1 << ": ";
                cin.ignore(); // Curăță buffer-ul înainte de a citi șirul
                getline(cin, operation); // Citeste operația ca un șir de caractere
                calculusStep->addOperation(operation);
            }
        }

        // Execută imediat pasul CALCULUS și afișează rezultatul
        calculusStep->execute();
        return calculusStep;
    }



    case 6:   // Pentru pasul DISPLAY
    {
        int index;
        string type;
        cout << "Enter step index: ";
        cin >> index;
        cin.ignore();
        cout << "Enter step type (TEXT FILE INPUT or CSV FILE INPUT): ";
        getline(cin, type);

        if ((type != "TEXT FILE INPUT" && type != "CSV FILE INPUT") ||
                (type == "TEXT FILE INPUT" && textFileInputInfo.find(index) == textFileInputInfo.end()) ||
                (type == "CSV FILE INPUT" && csvFileInputInfo.find(index) == csvFileInputInfo.end()))
        {
            cout << "Tip de pas necunoscut sau pas inexistent pentru indexul " << index << ".\n";
            return nullptr;
        }
        Display* displayStep = new Display(index, type);
        displayStep->displayImmediateInfo(); // Afișează informațiile imediat
        return new Display(index, type);
    }



    case 7:
    {
        cout << "Enter description: ";
        getline(cin, description);
        cout << "Enter file name: ";
        getline(cin, file_name);
        auto textFileInputStep = new TEXTFILEINPUTstep(description, file_name);
        textFileInputStep->execute(); // Asigură că pasul este executat și conținutul stocat
        return textFileInputStep;
    }


    case 8:
    {
        cout << "Enter description: ";
        getline(cin, description);
        cout << "Enter file name: ";
        getline(cin, file_name);
        auto csvFileInputStep = new CSVFILEINPUTstep(description, file_name);
        csvFileInputStep->execute(); // Asigură că pasul este executat și conținutul stocat
        return csvFileInputStep;
    }



    case 9:
    {
        cout << "Enter step: ";
        cin >> steps;
        cin.ignore();
        cout << "Enter file name: ";
        getline(cin, file_name);
        cout << "Enter title: ";
        getline(cin, title);
        cout << "Enter description: ";
        getline(cin, description);
        return new OUTPUTstep(steps, file_name, title, description);
    // Am adăugat break aici
        default:
            cout << "Invalid step type selected.\n";
            return nullptr;
        }


    case 10:  // Opțiunea pentru END Step
    {
        cout<<"This is the end of the flow. You should not add any more steps!" << endl;
        return new ENDstep();
    }
    }
}



int main()
{
    vector<Flow*> allFlows; // Utilizăm un vector de pointeri pentru a gestiona fluxurile
    string flowName;
    char choice;

    while (true)
    {
        cout << endl;
        cout << "1. Create a new flow\n";
        cout << "2. Display available steps\n";
        cout << "3. Delete a flow\n";
        cout << "4. Exit\n";
        cout << "5. Display step skip analytics\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore();



        if (choice == '1')
        {
            NUMBERINPUTstep::resetIdCounter();
            cout << "Enter flow name: ";
            getline(cin, flowName);
            Flow* newFlow = new Flow(flowName);

            char addMoreSteps;
            do
            {
                Step* step = createStep();
                if (step != nullptr)
                {
                    newFlow->addStep(step);
                }

                cout << endl;
                cout << "Add more steps? (y/n): ";
                cin >> addMoreSteps;
                cin.ignore();
            }
            while (addMoreSteps == 'y' || addMoreSteps == 'Y');


            char runFlow;
            cout << endl;
            cout << "Do you want to run this flow? (y/n): ";
            cin >> runFlow;
            if (runFlow == 'y' || runFlow == 'Y')
            {
                newFlow->executeFlow();
            }

// Adăugăm fluxul creat în vectorul allFlows
            allFlows.push_back(newFlow);
        }


        else if (choice == '2')
        {
            displayAvailableSteps();
        }


        else if (choice == '3')
        {
            if (allFlows.empty())
            {
                cout << "No flows available to delete." << endl;
                continue;
            }

            for (int i = 0; i < allFlows.size(); ++i)
            {
                cout << i << ": " << allFlows[i]->getFlowName() << endl;
            }

            int flowIndex;
            cout << "Enter the index of the flow to delete: ";
            cin >> flowIndex;
            cin.ignore();

            if (flowIndex >= 0 && flowIndex < allFlows.size())
            {
                delete allFlows[flowIndex]; // Eliberăm memoria alocată pentru fluxul selectat
                allFlows.erase(allFlows.begin() + flowIndex); // Ștergem pointerul din vector
                cout << "Flow deleted successfully." << endl;
            }
            else
            {
                cout << "Invalid flow index." << endl;
            }
        }



        else if (choice == '5')
        {
            for (const auto& flow : allFlows)
            {
                flow->displayStepSkipAnalytics();
            }
        }
        else if (choice == '4')
        {
            // Eliberați memoria și ieșiți din program
            for (auto& flow : allFlows)
            {
                delete flow;
            }
            break;
        }
    }

    return 0;
}
