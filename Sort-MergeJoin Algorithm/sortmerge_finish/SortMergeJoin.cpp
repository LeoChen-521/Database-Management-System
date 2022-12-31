#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;

struct EmpBlock {
    int eid;
    string ename;
    int age;
    double salary;
};

struct DeptBlock {
    int did;
    string dname;
    double budget;
    int managerid;
};


// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin) {
    string line, word;
    DeptBlock dept;
    if (getline(deptin, line, '\n')) {
        stringstream s(line);
        getline(s, word,',');
        dept.did = stoi(word);
        getline(s, word, ',');
        dept.dname = word;
        getline(s, word, ',');
        dept.budget = stod(word);
        getline(s, word, ',');
        dept.managerid = stoi(word);
        return dept;
    } else {
        dept.did = -1;
        return dept;
    }
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout) {
    fout << emp.eid << ',' << emp.ename << ',' << emp.age << ','
        << emp.salary << ',' << dept.did << ',' << dept.dname << ','
        << dept.budget << "\n";
}

bool emp_cmp(const EmpBlock &a, const EmpBlock &b)
{
    return a.eid < b.eid;
}

bool dept_cmp(const DeptBlock &a, const DeptBlock &b)
{
    return a.managerid < b.managerid;
}

void showEmp(vector <EmpBlock> eblocks)
{
    for(int i = 0; i < eblocks.size(); i++)
    {
        cout << i << ": " << eblocks[i].eid << endl;
    }
}

void emp_buffer(vector <EmpBlock> eblocks, fstream &buf)
{
    //fstream buf;
    //buf.open("emp_buf" + to_string(run - 1) + ".csv", ios::out | ios::app);
    for(int i = 0; i < eblocks.size(); i++)
    {
        buf << eblocks[i].eid    << ","
            << eblocks[i].ename  << ","
            << eblocks[i].age    << ","
            << eblocks[i].salary
            << "\n";
    }
}

void dept_buffer(vector <DeptBlock> dblocks, fstream &buf)
{
    //fstream buf;
    //buf.open("dept_buf" + to_string(run - 1) + ".csv", ios::out | ios::app);
    for(int i = 0; i < dblocks.size(); i++)
    {
        buf << dblocks[i].did    << ","
            << dblocks[i].dname  << ","
            << dblocks[i].budget    << ","
            << dblocks[i].managerid
            << "\n";
    }
}

EmpBlock emp_spcline(int line)
{
    int count = 0;
    EmpBlock b;
    fstream buf;
    buf.open("emp_buf.csv", ios::in);
    while(true) {
        b = grabEmp(buf);
        //cout << b.eid << endl;
        if(b.eid == -1)
            break;
        if(count == line)
            return b;
        count++;
    }
    return b;
}

DeptBlock dept_spcline(int line)
{
    int count = 0;
    DeptBlock b;
    fstream buf;
    buf.open("dept_buf.csv", ios::in);
    while(true) {
        b = grabDept(buf);
        if(b.did == -1)
            break;
        if(count == line)
            return b;
        count++;
    }
    return b;
}


int main() {
  int emp_total = 0;
  int dept_total = 0;
  int emp_min_ind = 0;
  int dept_min_ind = 0;
  int emp_min_eid = 0;
  int dept_min_mid = 0;
  bool tail = false;
  vector <int>        emp_seek;
  vector <int>        dept_seek;
  vector <EmpBlock>   eblocks;
  vector <DeptBlock>  dblocks;
  // open file streams to read and write
  fstream empin;
  fstream deptin;
  fstream joinout;
  fstream emp_buf;
  fstream dept_buf;
  deptin.open("Dept.csv", ios::in);
  empin.open("Emp.csv", ios::in);
  joinout.open("Join.csv", ios::out | ios::app);
  emp_buf.open("emp_buf.csv", ios::out | ios::app);
  dept_buf.open("dept_buf.csv", ios::out | ios::app);
  EmpBlock  empblock;
  DeptBlock deptblock;

  // flags check when relations are done being read
  bool flag = true;
  while (flag) {
      // for block in relation emp

      // grabs a block
      empblock = grabEmp(empin);
      if (empblock.eid == -1) {
          emp_total += eblocks.size();
          sort(eblocks.begin(), eblocks.end(), emp_cmp);
          emp_buffer(eblocks, emp_buf);
          eblocks.clear();
          flag = false;
          break;
      }
      eblocks.push_back(empblock);
      // checks if filestream is empty
      if (eblocks.size() % 22 == 0 && eblocks.size() != 0) {
          emp_total += eblocks.size();
          sort(eblocks.begin(), eblocks.end(), emp_cmp);
          emp_buffer(eblocks, emp_buf);
          eblocks.clear();
      }
  }
  //cout << emp_total << endl;
  flag = true;
  while(flag) {
      // for block in relation emp

      // grabs a block
      DeptBlock deptblock = grabDept(deptin);
      if (deptblock.did == -1) {
          dept_total += dblocks.size();
          sort(dblocks.begin(), dblocks.end(), dept_cmp);
          dept_buffer(dblocks, dept_buf);
          dblocks.clear();
          flag = false;
          break;
      }
      dblocks.push_back(deptblock);
      // checks if filestream is empty
      if (dblocks.size() % 22 == 0 && dblocks.size() != 0) {
          dept_total += dblocks.size();
          sort(dblocks.begin(), dblocks.end(), dept_cmp);
          dept_buffer(dblocks, dept_buf);
          dblocks.clear();
      }
  }
  emp_buf.close();
  dept_buf.close();
  for(int i = 0; i < emp_total; i++) {
      if(i % 22 == 0)
          emp_seek.push_back(i);
  }
  for(int i = 0; i < dept_total; i++) {
      if(i % 22 == 0)
          dept_seek.push_back(i);
  }
  //for(int i = 0; i < emp_seek.size(); i++)
    //cout << emp_seek[i] << endl;

  for(int i = 0; i < emp_seek.size(); i++) {
    eblocks.push_back(emp_spcline(emp_seek[i]));
  }
  for(int i = 0; i < dept_seek.size(); i++) {
    dblocks.push_back(dept_spcline(dept_seek[i]));
  }
  //flag = true;
  while(true) {
     //showEmp(eblocks);
    for(int i = 0; i < emp_seek.size(); i++) {
        if(eblocks[i].eid < eblocks[emp_min_ind].eid)
            emp_min_ind = i;
    }
    emp_min_eid = eblocks[emp_min_ind].eid;
    if (emp_min_eid == -1 && tail == false) {
        emp_min_ind--;
        emp_seek.pop_back();
        eblocks.pop_back();
        tail = true;
        //cout << "tail" << endl;
        continue;
    }
    for(int i = 0; i < dept_seek.size(); i++) {
        if(dblocks[i].managerid < dblocks[dept_min_ind].managerid)
            dept_min_ind = i;
    }
    dept_min_mid = dblocks[dept_min_ind].managerid;
    if (dblocks[dept_min_ind].did == -1)
        return 0;
    /*
        dept_buf.close();
        dblocks.clear();
        dept_seek.clear();
        dept_buf.open("dept_buf.csv", ios::out | ios::app);

        for(int i = 0; i < dept_total; i++) {
            if(i % 22 == 0)
                dept_seek.push_back(i);
        }
        for(int i = 0; i < dept_seek.size(); i++)
            dblocks.push_back(dept_spcline(dept_seek[i]));

        for(int i = 0; i < dept_seek.size(); i++) {
            if(dblocks[i].managerid < dblocks[dept_min_ind].managerid)
                dept_min_ind = i;
        }
        dept_min_mid = dblocks[dept_min_ind].managerid;
    }
    */
    //cout << "dept min ind: " << dept_min_mid << "\t" <<  dept_seek[dept_min_ind] << endl;

    //if (emp_min_eid != -1) {
        if (emp_min_eid > dept_min_mid) {
            dblocks.erase(dblocks.begin() + dept_min_ind);
            dept_seek[dept_min_ind]++;
            dblocks.insert(dblocks.begin() + dept_min_ind, dept_spcline(dept_seek[dept_min_ind]));
        }
        else if (emp_min_eid < dept_min_mid) {
            eblocks.erase(eblocks.begin() + emp_min_ind);
            emp_seek[emp_min_ind]++;
            eblocks.insert(eblocks.begin() + emp_min_ind, emp_spcline(emp_seek[emp_min_ind]));
        }
        else {
            printJoin(eblocks[emp_min_ind], dblocks[dept_min_ind], joinout);
            //eblocks.erase(eblocks.begin() + emp_min_ind);
            dblocks.erase(dblocks.begin() + dept_min_ind);
            //emp_seek[emp_min_ind]++;
            //eblocks.insert(eblocks.begin() + emp_min_ind, emp_spcline(emp_seek[emp_min_ind]));
            dept_seek[dept_min_ind]++;
            dblocks.insert(dblocks.begin() + dept_min_ind, dept_spcline(dept_seek[dept_min_ind]));
        }
    //}
    //else
        //break;
     //showEmp(eblocks);
  }
/*
  int emp_runs_num;
  int dept_runs_num;
  fstream* buf;
  vector <fstream*>  emp_runs;
  vector <fstream*> dept_runs;
  if(emp_total % 22 == 0)
    emp_runs_num = emp_total / 22;
  else
    emp_runs_num = emp_total / 22 + 1;
  if(dept_total % 22 == 0)
    dept_runs_num = dept_total / 22;
  else
    dept_runs_num = dept_total / 22 + 1;
  for(int i = 0; i < emp_runs_num; i++) {
    buf->open("emp_buf" + to_string(i) + ".csv", ios::in);
    emp_runs.push_back(buf);
    empblock = grabEmp(*buf);
    eblocks.push_back(empblock);
    //fstream buf1("emp_buf" + to_string(i) + ".csv", ios::in);
    //empblock = grabEmp(buf1);
    //eblocks.push_back(empblock);
    showEmp(eblocks);
  }
    for(int i = 0; i < dept_runs_num; i++) {
    buf.open("dept_buf" + to_string(i) + ".csv", ios::in);
    dept_runs.push_back(&buf);
  }
  //empblock = grabEmp(*emp_runs[0]);
  for(int i = 0; i < emp_runs_num; i++) {
    empblock = grabEmp(*emp_runs[i]);
    eblocks.push_back(empblock);
  }
  showEmp(eblocks);
    for(int i = 0; i < dept_runs_num; i++) {
    deptblock = grabDept(*dept_runs[i]);
    dblocks.push_back(deptblock);
  }


  while(true) {
    int  emp_min_index = max_element(eblocks.begin(), eblocks.end()) - eblocks.begin();
    int dept_min_index = max_element(dblocks.begin(), dblocks.end()) - dblocks.begin();
    int  emp_min = min_element(eblocks.begin(), eblocks.end());

    if(emp_min.eid > dept_min.managerid) {
        dblocks.erase(dblocks[0]);
    }
  }
*/

  return 0;
}
