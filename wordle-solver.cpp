#include <bits/stdc++.h>

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

vector<string> get_dict() {
  ifstream in("wordle-allowed-guesses.txt");
  vector<string> v;
  string s;
  while (in >> s) {
    v.push_back(s);
  }
  in.close();
  shuffle(begin(v), end(v), rng);
  stable_sort(begin(v), end(v), [](auto& s, auto& t) {
    set<char> S, T;
    for (char c : s) S.insert(c);
    for (char c : t) T.insert(c);
    return S.size() > T.size();
  });
  return v;
}

// check if s is valid under {t, f}
bool ok(string s, string t, string f) {
  for (int i = 0; i < 5; i++) {
    if (f[i] == '!' && s[i] != t[i]) return false;
    if (f[i] == 'o' && s[i] == t[i]) return false;
  }
  set<char> now, ban, have;
  for (int i = 0; i < 5; i++) {
    now.insert(s[i]);
    if (f[i] == 'x') {
      ban.insert(t[i]);
    } else if (f[i] == 'o') {
      have.insert(t[i]);
    }
  }
  for (char c : ban) {
    if (now.count(c)) return false;
  }
  for (char c : have) {
    if (!now.count(c)) return false;
  }
  return true;
}

void update(vector<string>& dict, string t, string f) {
  vector<string> new_dict;
  for (const string& s : dict) {
    if (ok(s, t, f)) new_dict.push_back(s);
  }
  swap(dict, new_dict);
}

int main() {
  vector<string> dict = get_dict();
  string guess = "arise", result;
  cout << "? " << guess << " (1/" << dict.size() << ")" << endl;
  cin >> result;
  for (;;) {
    update(dict, guess, result);
    if (dict.size() < 1) break;
    guess = dict[0];
    cout << "? " << guess << " (1/" << dict.size() << ")" << endl;
    cin >> result;
    if (result == "!!!!!") break;
  }
  return 0;
}