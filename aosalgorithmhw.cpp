#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <vector>

using namespace std;

int s[300000], d[300000];
map<int, string> arb;
map<int, int> mine;
random_device rd;
default_random_engine gen = default_random_engine(rd());

void setrandomstring() {
    uniform_int_distribution<int> dis1(1, 20);
    uniform_int_distribution<int> dis2(1, 1200);  //referencestring
    //cout << "jkghk";
    for (int i = 0; i < 300000; i++) {
        int a = dis1(gen);
        int b = dis2(gen);
        for (int j = 0; i < 300000 && j < a; j++, i++) {
            if (b + j <= 1200) {
                s[i] = b + j;
            } else {
                s[i] = (b + j) % 1200 + 1;
            }
        }
        i--;
    }
    uniform_int_distribution<int> dis4(0, 1);
    for (int i = 0; i < 300000; i++) {
        d[i] = dis4(gen);
        cout << s[i] << " " << d[i] << endl;
    }
    //cout << "random" << endl;
}
void setlocalitystring() {
    int a = 300000 / 120;
    int b = 300000 / 300;
    uniform_int_distribution<int> dis1(b, a);
    uniform_int_distribution<int> dis2(1, 1200);
    uniform_int_distribution<int> dis3(1, 100);
    for (int i = 0; i < 300000; i++) {
        int programsize = dis1(gen);
        int head = dis2(gen);
        int length = dis3(gen);

        uniform_int_distribution<int> dis4(head, head + length);
        for (int j = 0; j < programsize && i < 300000; j++, i++) {
            int temp = dis4(gen);
            if (temp <= 1200) {
                s[i] = temp;
            } else {
                s[i] = temp % 1200 + 1;
            }
        }
        i--;
    }
    uniform_int_distribution<int> dis4(0, 1);
    for (int i = 0; i < 300000; i++) {
        d[i] = dis4(gen);
        cout << s[i] << " " << d[i] << endl;
    }
}
void setmystring() {
    for (int i = 0; i < 300000; i++) {
        uniform_int_distribution<int> dis2(1, 1100);
        uniform_int_distribution<int> dis4(0, 5);
        uniform_int_distribution<int> dis5(0, 10);
        int head = dis2(gen);
        vector<int> a;
        uniform_int_distribution<int> dis3(head, head + 100);
        for (int j = 0; j < 2000 && i < 300000; j++) {
            s[i] = dis3(gen);
            if (!dis4(gen)) {
                a.push_back(s[i]);
            }
            i++;
            if (dis5(gen) == 0) {
                for (int k = 0; k < a.size() && i < 300000; k++) {
                    s[i] = a[k];
                    ;
                    i++;
                }
            }
        }
    }
    uniform_int_distribution<int> dis6(0, 1);
    for (int i = 0; i < 300000; i++) {
        d[i] = dis6(gen);
        cout << s[i] << " " << d[i] << endl;
    }
    //cout << "mystring" << endl;
}
void fifo(int framenum) {
    queue<int> framedata;
    queue<int> dirty;
    int pagefault = 0, interrupt = -1, writeback = 0;
    set<int> tempdata;

    for (int i = 0; i < 300000; i++) {
        if (i % 1000 == 0) {
            interrupt++;
            writeback++;
            tempdata.clear();
        }
        if (framedata.size() < framenum) {
            int flag = 0, temp1, temp2, length = framedata.size();
            for (int j = 0; j < length; j++) {
                if (framedata.front() != s[i]) {
                    framedata.push(framedata.front());
                    framedata.pop();
                    dirty.push(dirty.front());
                    dirty.pop();
                } else {
                    flag = 1;
                    temp1 = framedata.front();
                    framedata.pop();
                    temp2 = dirty.front();
                    dirty.pop();
                }
            }
            if (flag == 0) {
                framedata.push(s[i]);
                dirty.push(d[i]);
            } else {
                pagefault++;
                framedata.push(temp1);
                dirty.push(temp2);
            }
        } else {
            int flag = 0, temp1, temp2;
            for (int j = 0; j < framenum; j++) {
                if (framedata.front() != s[i]) {
                    framedata.push(framedata.front());
                    framedata.pop();
                    dirty.push(dirty.front());
                    dirty.pop();
                } else {
                    flag = 1;
                    temp1 = framedata.front();
                    framedata.pop();
                    temp2 = dirty.front();
                    dirty.pop();
                }
            }
            if (flag == 0) {
                pagefault++;
                if (tempdata.count(s[i])) {
                    writeback++;
                    interrupt++;
                    tempdata.clear();
                }
                if (dirty.front() == 1) {
                    tempdata.insert(framedata.front());
                }
                framedata.pop();
                framedata.push(s[i]);
                dirty.pop();
                dirty.push(d[i]);
            } else {
                framedata.push(temp1);
                dirty.push(temp2);
            }
        }
    }
    cout << "pagefault=" << pagefault << "  interupt=" << interrupt << " writeback=" << writeback << "    fifo" << endl;
}
void fifo1(int framenum) {
    int times = 20, pagefault = 0, interupt = -1, writeback = 0;
    set<int> frame;
    vector<int> qu;
    set<int> tempdata;
    map<int, int> dirty;
    for (int i = 0; i < 300000; i++) {
        //定期檢查寫回
        if (i % 1000 == 0) {
            writeback++;
            interupt++;
            tempdata.clear();
        }
        if (frame.size() < framenum) {
            //不再frame裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                frame.insert(s[i]);
                qu.push_back(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
            } else {
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
                qu.push_back(s[i]);
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (*it == s[i]) {
                        qu.erase(it);
                        break;
                    }
                }
            }

        } else {
            //不再frame裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                vector<int>::iterator it1 = qu.begin();
                //victim是否需要被寫回
                if (dirty[*it1] == 1) {
                    tempdata.insert(*it1);
                    dirty[*it1] = 0;
                }  //心放進來的有沒有被寫過
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }  //是否要先寫回
                if (tempdata.count(s[i]) == 1) {
                    tempdata.clear();
                    interupt++;
                    writeback++;
                }
                frame.insert(s[i]);
                frame.erase(*it1);
                qu.erase(it1);
                qu.push_back(s[i]);
            } else {
                qu.push_back(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (*it == s[i]) {
                        qu.erase(it);
                        break;
                    }
                }
            }
        }
    }
    cout << "pagefault=" << pagefault << "  interupt=" << interupt << " writeback=" << writeback << "    fifo1" << endl;
}
void optimal(int framenum) {
    int interupt = -1, pagefault = 0, writeback = 0;
    set<int> frame;
    map<int, int> dirty;
    set<int> tempdata;
    for (int i = 0; i < 300000; i++) {
        if (i % 1000 == 0) {
            writeback++;
            interupt++;
            tempdata.clear();
        }
        //page還沒滿
        if (frame.size() < framenum) {
            //不在page裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                frame.insert(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
            }
            //在page裡
            else {
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
            }
        }
        //page滿了
        else {
            //不在page裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                //資料未寫回
                if (tempdata.count(s[i]) == 1) {
                    interupt++;
                    writeback++;
                    tempdata.clear();
                }
                //dirtybits
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
                set<int> findvictim;
                for (set<int>::iterator it = frame.begin(); it != frame.end(); it++) {
                    findvictim.insert(*it);
                }
                //找victimpage
                interupt++;
                for (int j = i + 1; findvictim.size() > 1 && j < 300000; j++) {
                    if (findvictim.count(s[j]) == 1) {
                        findvictim.erase(s[j]);
                    }
                }
                if (dirty[*findvictim.begin()] == 1) {
                    tempdata.insert(*findvictim.begin());
                }
                frame.erase(*findvictim.begin());
                dirty[*findvictim.begin()] = 0;
                frame.insert(s[i]);

            }
            //在page裡
            else {
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
            }
        }
    }
    cout << "pagefault=" << pagefault << "  interupt=" << interupt << " writeback=" << writeback << "     opt" << endl;
}
void Additionalreferencebits(int framenum) {
    set<int> frame;
    set<int> tempdata;
    set<int> lasttimes;
    map<int, int> dirty;
    vector<int> qu;
    int times = 10;
    int interupt = -1, pagefault = 0, writeback = 0;
    for (int i = 0; i < 300000; i++) {
        //定期檢查寫回
        if (i % 1000 == 0) {
            writeback++;
            interupt++;
            tempdata.clear();
            dirty.clear();
        }
        if (frame.size() < framenum) {
            //不在裡面
            if (frame.count(s[i]) == 0) {
                pagefault++;
                lasttimes.insert(s[i]);
                frame.insert(s[i]);
                qu.push_back(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
            } else {
                qu.push_back(s[i]);
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (*it == s[i]) {
                        qu.erase(it);
                        break;
                    }
                }
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
            }
        }
        //page滿了
        else {
            //不再裡面
            if (frame.count(s[i]) == 0) {
                pagefault++;
                int leftest = 1;
                set<int> same;
                lasttimes.insert(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
                same.insert(*qu.begin());
                //第一個得最左邊0
                int temp = 8;
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    temp = 8;
                    for (int j = 0; j < arb[*it].length(); j++) {
                        if (arb[*it][j] == '1') {
                            temp = j;
                            break;
                        }
                    }
                    if (temp == leftest) {
                        same.insert(*it);
                    } else if (temp > leftest) {
                        same.clear();
                        same.insert(*it);
                        leftest = temp;
                    }
                }
                int k = 1;
                leftest = 7;
                while (same.size() > 1 && k <= 7) {
                    for (set<int>::iterator it = same.begin(); it != same.end(); it++) {
                        temp = 8;
                        for (int j = k; j <= 7; j++) {
                            if (arb[*it][j] == '1') {
                                temp = j;
                                break;
                            }
                        }
                        if (temp == leftest) {
                            same.insert(*it);
                        } else if (temp > leftest) {
                            same.clear();
                            same.insert(*it);
                            leftest = temp;
                        }
                    }
                    k++;
                }
                //fifo找victim
                if (same.size() > 1) {
                    bool flag = false;
                    vector<int>::iterator it;
                    for (vector<int>::iterator it1 = qu.begin(); it1 != qu.end(); it1++) {
                        for (set<int>::iterator it2 = same.begin(); it2 != same.end(); it2++) {
                            if (*it1 == *it2) {
                                it = it1;
                                flag = true;
                                break;
                            }
                        }
                        if (flag) {
                            break;
                        }
                    }  //check dirtybit
                    if (dirty[*it] == 1) {
                        tempdata.insert(*it);
                    }
                    frame.erase(*it);
                    qu.erase(it);
                    qu.push_back(s[i]);
                    frame.insert(s[i]);
                    //是否要先寫回disk
                    if (tempdata.count(s[i]) == 1) {
                        tempdata.clear();
                        interupt++;
                        writeback++;
                    }
                }  //arb找到vicitm
                else {
                    set<int>::iterator it = same.begin();
                    for (vector<int>::iterator it1 = qu.begin(); it1 != qu.end(); it1++) {
                        if (*it == *it1) {
                            if (d[*it] == 1) {
                                tempdata.insert(*it);
                            }
                            frame.erase(*it);
                            frame.insert(s[i]);
                            qu.erase(it1);
                            qu.push_back(s[i]);
                            if (tempdata.count(s[i]) == 1) {
                                tempdata.clear();
                                interupt++;
                            }

                            break;
                        }
                    }
                }

            }  //在裡面
            else {
                lasttimes.insert(s[i]);
                for (vector<int>::iterator it1 = qu.begin(); it1 != qu.end(); it1++) {
                    if (*it1 == s[i]) {
                        qu.erase(it1);
                        qu.push_back(s[i]);
                        break;
                    }
                }
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
            }
        }  //shift

        if (i % times == 0) {
            interupt++;
            set<int> count1;
            //shift第一個

            arb[s[i]] = "1" + arb[s[i]].substr(0, 7);
            count1.insert(s[i]);
            //shift前8*times個
            for (int j = i - 1; j >= 0 && j > i - (8 * times); j--) {
                if (count1.count(s[j]) == 0) {
                    //前面30次沒出現
                    if (lasttimes.count(s[j]) == 0) {
                        lasttimes.insert(s[j]);
                        arb[s[j]] = "0" + arb[s[j]].substr(0, 7);
                        count1.insert(s[j]);
                    } else {
                        lasttimes.insert(s[j]);

                        arb[s[j]] = "1" + arb[s[j]].substr(0, 7);
                        //cout << arb[s[j]] << endl;
                        count1.insert(s[j]);
                    }
                }
            }
            //清掉前30個reference
            lasttimes.clear();
        }
    }
    cout << "pagefault=" << pagefault << "  interupt=" << interupt << " writeback=" << writeback << "    arb" << endl;
}
void myalgorithm(int framenum) {
    int times = 20, pagefault = 0, interupt = -1, writeback = 0;
    int pagefaultcount = 0;
    set<int> frame;
    vector<int> qu;
    vector<int> countlru;
    vector<int> counttimes;
    set<int> tempdata;
    map<int, int> dirty;
    for (int i = 0; i < 300000; i++) {
        //定期檢查寫回
        if (i % 1000 == 0) {
            writeback++;
            interupt++;
            tempdata.clear();
        }
        if (frame.size() < framenum) {
            //不再frame裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                frame.insert(s[i]);
                qu.push_back(s[i]);
                countlru.push_back(s[i]);
                counttimes.push_back(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }

            } else {
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
                qu.push_back(s[i]);
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (*it == s[i]) {
                        qu.erase(it);
                        break;
                    }
                }
                countlru.push_back(s[i]);
                counttimes.push_back(s[i]);
            }

        } else {
            //不再frame裡
            if (frame.count(s[i]) == 0) {
                pagefault++;
                vector<int>::iterator it1;
                int smallest = 9999999;
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (mine[*it] < smallest) {
                        smallest = mine[*it];
                        it1 = it;
                    }
                }
                if (dirty[*it1] == 1) {
                    tempdata.insert(*it1);
                    dirty[*it1] = 0;
                }
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                } else {
                    dirty[s[i]] = 0;
                }
                //是否要先寫回
                if (tempdata.count(s[i]) == 1) {
                    tempdata.clear();
                    interupt++;
                    writeback++;
                }
                frame.insert(s[i]);
                frame.erase(*it1);
                qu.erase(it1);
                qu.push_back(s[i]);
                countlru.push_back(s[i]);
                counttimes.push_back(s[i]);

            } else {
                qu.push_back(s[i]);
                if (d[i] == 1) {
                    dirty[s[i]] = 1;
                }
                for (vector<int>::iterator it = qu.begin(); it != qu.end(); it++) {
                    if (*it == s[i]) {
                        qu.erase(it);
                        break;
                    }
                }
                countlru.push_back(s[i]);
                counttimes.push_back(s[i]);
            }
        }
        set<int> alreadypush;
        if (i % times == 0) {
            interupt++;
            for (int j = 0; j < counttimes.size(); j++) {
                mine[counttimes[j]] += 10;
                if (alreadypush.count(countlru[j]) == 0) {
                    mine[countlru[j]] += 1;
                    alreadypush.insert(countlru[j]);
                }
            }
            alreadypush.clear();
            countlru.clear();
            counttimes.clear();
        }

        if (i % 1000 == 0) {
            for (int j = 0; j <= 1200; j++) {
                mine[j] = 0;
            }
        }
    }
    cout << "pagefault=" << pagefault << "  interupt=" << interupt << " writeback=" << writeback << "    mine" << endl;
}
int main() {
    int k;
    string c;
    while (k = 1) {
        int a, b, framenum;
        cout << "enter string:  1=random   2=locality   3=mystring" << endl;
        cin >> a;
        cout << "enter algorithm: 1=fifo  2=optimal  3=arb  4=myalgorithm" << endl;
        cin >> b;
        cout << "enter framenumber: " << endl;
        cin >> framenum;

        if (a == 1) {
            c = "D:\\random.txt";
            //setrandomstring();
        } else if (a == 2) {
            c = "D:\\locality.txt";
            //setlocalitystring();
        } else {
            c = "D:\\mystring.txt";
            //setmystring();
        }
        ifstream fin(c);
        for (int i = 0; i < 30000; i++) {
            fin >> s[i] >> d[i];
        }
        fin.close();
        if (b == 1) {
            fifo1(framenum);
        } else if (b == 2) {
            optimal(framenum);
        } else if (b == 3) {
            for (int i = 0; i < 1200; i++) {
                arb[i] = "00000000";
            }
            Additionalreferencebits(framenum);
        } else {
            for (int i = 0; i < 1200; i++) {
                mine[i] = 0;
            }
            myalgorithm(framenum);
        }
    }
    return 0;
}