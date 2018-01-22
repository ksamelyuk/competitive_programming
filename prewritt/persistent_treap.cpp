// Solution for http://codeforces.com/contest/916/problem/D

#include <iostream>
#include <algorithm>
#include <bitset>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <iomanip>
#include <queue>
#include <utility>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<int, char> pci;

template <class TKey, class TValue>
struct Node {
  TKey key;
  TValue value;
  int priority;
  int count;
  Node* left;
  Node* right;

  Node(const TKey& _key, const TValue& _value, Node* _left = nullptr, Node* _right = nullptr)
      : key(_key), value(_value), priority(rand()), count(1), left(_left), right(_right) {}
};

template <class TKey, class TValue>
using NodePtr = Node<TKey, TValue>*;

template <class TKey, class TValue>
int Count(NodePtr<TKey, TValue> root) {
  return root ? root->count : 0;
}

template <class TKey, class TValue>
void Update(NodePtr<TKey, TValue> root) {
  if (!root) {
    return;
  }
  root->count = 1 + Count(root->left) + Count(root->right);
}

template <class TKey, class TValue>
void Split(NodePtr<TKey, TValue> root, const TKey& key, NodePtr<TKey, TValue>& left, NodePtr<TKey, TValue>& right) {
  if (!root) {
    left = right = nullptr;
    return;
  }
  root = new Node<TKey, TValue>(*root);
  if (key < root->key) {
    Split(root->left, key, left, root->left);
    right = root;
  } else {
    Split(root->right, key, root->right, right);
    left = root;
  }
  Update(root);
}

template <class TKey, class TValue>
NodePtr<TKey, TValue> Merge(NodePtr<TKey, TValue> left, NodePtr<TKey, TValue> right) {
  if (!left || !right) {
    return left ? left : right;
  }
  NodePtr<TKey, TValue> root = nullptr;
  if (left->priority > right->priority) {
    root = new Node<TKey, TValue>(*left);
    root->right = Merge(left->right, right);
  } else {
    root = new Node<TKey, TValue>(*right);
    root->left = Merge(left, right->left);
  }
  Update(root);
  return root;
}

template <class TKey, class TValue>
NodePtr<TKey, TValue> Insert(NodePtr<TKey, TValue> root, NodePtr<TKey, TValue> item) {
  if (!root) {
    return item;
  }
  if (item->priority > root->priority) {
    Split(root, item->key, item->left, item->right);
    Update(item);
    return item;
  }
  root = new Node<TKey, TValue>(*root);
  if (item->key < root->key) {
    root->left = Insert(root->left, item);
  } else {
    root->right = Insert(root->right, item);
  }
  Update(root);
  return root;
}

template <class TKey, class TValue>
NodePtr<TKey, TValue> Find(NodePtr<TKey, TValue> root, const TKey& key) {
  if (!root || root->key == key) {
    return root;
  }
  return Find(key < root->key ? root->left : root->right, key);
}

template <class TKey, class TValue>
NodePtr<TKey, TValue> Erase(NodePtr<TKey, TValue> root, const TKey& key) {
  if (!root) {
    return nullptr;
  }
  root = new Node<TKey, TValue>(*root);
  if (root->key == key) {
    root = Merge(root->left, root->right);
  } else if (key < root->key) {
    root->left = Erase(root->left, key);
  } else {
    root->right = Erase(root->right, key);
  }
  Update(root);
  return root;
}

template <class TKey, class TValue>
int Count(NodePtr<TKey, TValue> root, const TKey& key) {
  if (!root) {
    return 0;
  }
  return key < root->key ? Count(root->left, key) : 1 + Count(root->left) + Count(root->right, key);
}

void Solve() {
  int m; cin >> m;
  using SetIntString = Node<pair<int, string>, bool>;
  using MapStringInt = Node<string, int>;
  MapStringInt* map_a = nullptr;
  vector<MapStringInt*> maps_a = { map_a };
  SetIntString* map_b = nullptr;
  vector<SetIntString*> maps_b = { map_b };
  for (int i = 1; i <= m; ++i) {
    string type; cin >> type;
    if (type == "set") {
      string action; int priority; cin >> action >> priority;
      auto find = Find(map_a, action);
      if (find) {
        map_a = Erase(map_a, find->key);
        map_b = Erase(map_b, make_pair(find->value, find->key));
      }
      map_a = Insert(map_a, new MapStringInt(action, priority));
      map_b = Insert(map_b, new SetIntString(make_pair(priority, action), true));
    } else if (type == "remove") {
      string action; cin >> action;
      auto find = Find(map_a, action);
      if (find) {
        map_a = Erase(map_a, find->key);
        map_b = Erase(map_b, make_pair(find->value, find->key));
      }
    } else if (type == "query") {
      string action; cin >> action;
      auto find = Find(map_a, action);
      if (!find) {
        cout << -1 << endl;
      } else {
        cout << Count(map_b, make_pair(find->value - 1, string(16, 'z'))) << endl;
      }
    } else {
      int delta_days; cin >> delta_days;
      map_a = maps_a[i - 1 - delta_days];
      map_b = maps_b[i - 1 - delta_days];
    }
    maps_a.push_back(map_a);
    maps_b.push_back(map_b);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  Solve();
  return 0;
}
