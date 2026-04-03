#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <mutex>
#include <windows.h>

using namespace std;

mutex mtx;

// ==================== ÊËÀÑÑ ÊÎÌÀÍÄÀ ====================
class Team {
public:
    string name;
    int fighters;
    int kills;
    bool alive;
    int priority;

    Team(string n, int f, int p) {
        name = n;
        fighters = f;
        kills = 0;
        alive = true;
        priority = p;
    }
};

// ==================== ÔÓÍÊÖÈß ÄËß ÎÒĞÈÑÎÂÊÈ ÏÎËÎÑÛ ====================
void drawBar(int current) {
    for (int i = 0; i < current; i++) {
        cout << "#";
    }
    for (int i = current; i < 20; i++) {
        cout << "-";
    }
}

// ==================== ÔÓÍÊÖÈß ÁÈÒÂÛ ====================
void battle(vector<Team>& teams, int index) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> addDist(1, 5);
    uniform_int_distribution<> killDist(2, 10);

    while (true) {
        int delay = 1200 - teams[index].priority * 30;
        if (delay < 200) delay = 200;
        this_thread::sleep_for(chrono::milliseconds(delay));
        
        lock_guard<mutex> lock(mtx);

        // ïğîâåğÿåì, æèâà ëè êîìàíäà
        if (!teams[index].alive) break;

        // ïğîâåğÿåì, åñòü ëè æèâûå ïğîòèâíèêè
        vector<int> aliveTargets;
        for (int i = 0; i < teams.size(); i++) {
            if (i != index && teams[i].alive) {
                aliveTargets.push_back(i);
            }
        }
        if (aliveTargets.empty()) break;

        int newFighters = addDist(gen);
        teams[index].fighters += newFighters;
        if (teams[index].fighters > 20) teams[index].fighters = 20;

        uniform_int_distribution<> targetDist(0, aliveTargets.size() - 1);
        int target = aliveTargets[targetDist(gen)];

        int killed = killDist(gen);
        int actualKilled = min(killed, teams[target].fighters);

        teams[target].fighters -= actualKilled;
        teams[index].kills += actualKilled;

        if (teams[target].fighters == 0) {
            teams[target].alive = false;
        }

        // âûâîä
        cout << "\n----------------------------------------------\n";
        cout << "Ïîòîê êîìàíäû: " << teams[index].name;
        cout << "\nÏğèîğèòåò: " << teams[index].priority;
        cout << "\n     Ïğèğîñò: +" << newFighters;
        cout << "\n     Àòàêà íà: " << teams[target].name;
        cout << "\n     Óáèòî: " << actualKilled;
        cout << "\n     Áîéöîâ: " << teams[index].fighters << "/20";
        cout << "\n----------------------------------------------\n";
    }

    // ñîîáùåíèå î ïîğàæåíèè
    mtx.lock();
    if (!teams[index].alive) {
        cout << "\n=============================================" << endl;
        cout << "ÊÎÌÀÍÄÀ " << teams[index].name << " ÏÎÒÅĞÏÅËÀ ÏÎĞÀÆÅÍÈÅ" << endl;
        cout << "=============================================" << endl;
    }
    mtx.unlock();
}

// ==================== ÓÑÒÀÍÎÂÊÀ ÏĞÈÎĞÈÒÅÒÀ ====================
void setPriority(thread& t, int priority) {
    HANDLE h = t.native_handle();
    if (priority <= 10) {
        SetThreadPriority(h, THREAD_PRIORITY_LOWEST);
    }
    else if (priority <= 20) {
        SetThreadPriority(h, THREAD_PRIORITY_NORMAL);
    }
    else {
        SetThreadPriority(h, THREAD_PRIORITY_HIGHEST);
    }
}

// ==================== ÃËÀÂÍÀß ÔÓÍÊÖÈß ====================
int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=============================================" << endl;
    cout << "ÁÈÒÂÀ ÊÎÌÀÍÄ (ÌÍÎÃÎÏÎÒÎ×ÍÎÅ ÏĞÎÃĞÀÌÌÈĞÎÂÀÍÈÅ)" << endl;
    cout << "=============================================" << endl;

    int teamCount;
    cout << "\nÂâåäèòå êîëè÷åñòâî êîìàíä (2-5): ";
    cin >> teamCount;

    if (teamCount < 2) teamCount = 2;
    if (teamCount > 5) teamCount = 5;

    vector<Team> teams;
    vector<thread> threads;

    for (int i = 0; i < teamCount; i++) {
        string name;
        int fighters;
        int priority;

        cout << "\n---------------------------------------------" << endl;
        cout << "Êîìàíäà " << i + 1 << endl;
        cout << "---------------------------------------------" << endl;
        cout << "Íàçâàíèå: ";
        cin >> name;
        cout << "Íà÷àëüíîå êîëè÷åñòâî áîéöîâ (1-20): ";
        cin >> fighters;
        if (fighters < 1) fighters = 1;
        if (fighters > 20) fighters = 20;
        cout << "Ïğèîğèòåò ïîòîêà (1-30): ";
        cin >> priority;

        teams.push_back(Team(name, fighters, priority));
    }

    cout << "\n=============================================" << endl;
    cout << "ÍÀ×ÀËÎ ÁÈÒÂÛ" << endl;
    cout << "=============================================" << endl;

    for (int i = 0; i < teamCount; i++) {
        threads.push_back(thread(battle, ref(teams), i));
    }

    for (int i = 0; i < teamCount; i++) {
        setPriority(threads[i], teams[i].priority);
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "\n=============================================" << endl;
    cout << "ĞÅÇÓËÜÒÀÒÛ ÁÈÒÂÛ" << endl;
    cout << "=============================================" << endl;

    int winner = -1;

    for (int i = 0; i < teamCount; i++) {
        cout << "\n---------------------------------------------" << endl;
        cout << teams[i].name << endl;
        cout << "---------------------------------------------" << endl;
        cout << "Áîéöîâ îñòàëîñü: " << teams[i].fighters << "/20" << endl;
        cout << "Óíè÷òîæåíî âğàãîâ: " << teams[i].kills << endl;
        cout << "Ïîëîñà çäîğîâüÿ: [";
        drawBar(teams[i].fighters);
        cout << "]" << endl;

        if (teams[i].alive) {
            cout << "Ñòàòóñ: ÂÛÆÈËÀ" << endl;
            winner = i;
        }
        else {
            cout << "Ñòàòóñ: ÓÍÈ×ÒÎÆÅÍÀ" << endl;
        }
        cout << "---------------------------------------------" << endl;
    }

    cout << "\n=============================================" << endl;
    if (winner != -1) {
        cout << "ÏÎÁÅÄÈÒÅËÜ: " << teams[winner].name << endl;
    }
    else {
        cout << "ÍÈ×Üß. ÂÑÅ ÊÎÌÀÍÄÛ ÓÍÈ×ÒÎÆÅÍÛ" << endl;
    }
    cout << "=============================================" << endl;

    system("pause");
    return 0;
}