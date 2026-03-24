#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

struct ListNode {
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
    
    ListNode(const std::string& str = "") : data(str) {}
};

std::pair<std::string, int> parseLine(const std::string& line) {
    size_t pos = line.find(';');
    if (pos == std::string::npos) {
        return {"", -1};
    }
    
    std::string data = line.substr(0, pos);
    std::string indexStr = line.substr(pos + 1);
    int randIndex = std::stoi(indexStr);
    
    return {data, randIndex};
}

ListNode* deserializeFromText(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return nullptr;
    }
    
    std::vector<std::string> dataVector;
    std::vector<int> randIndices;
    std::string line;
    
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        
        auto [data, randIndex] = parseLine(line);
        dataVector.push_back(data);
        randIndices.push_back(randIndex);
    }
    
    in.close();
    
    int n = dataVector.size();
    if (n == 0) {
        return nullptr;
    }
    
    std::vector<ListNode*> nodes(n);
    for (int i = 0; i < n; ++i) {
        nodes[i] = new ListNode(dataVector[i]);
    }
    
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            nodes[i]->prev = nodes[i - 1];
        }
        if (i < n - 1) {
            nodes[i]->next = nodes[i + 1];
        }
    }
    
    for (int i = 0; i < n; ++i) {
        if (randIndices[i] != -1 && randIndices[i] >= 0 && randIndices[i] < n) {
            nodes[i]->rand = nodes[randIndices[i]];
        }
    }
    
    return nodes[0];
}

void serializeToBinary(ListNode* head, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Cannot create file: " << filename << std::endl;
        return;
    }
    
    if (!head) {
        int n = 0;
        out.write(reinterpret_cast<const char*>(&n), sizeof(n));
        out.close();
        return;
    }
    
    std::vector<ListNode*> nodes;
    ListNode* current = head;
    while (current) {
        nodes.push_back(current);
        current = current->next;
    }
    
    int n = nodes.size();
    
    std::map<ListNode*, int> nodeToIndex;
    for (int i = 0; i < n; ++i) {
        nodeToIndex[nodes[i]] = i;
    }
    
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    
    for (int i = 0; i < n; ++i) {
        int prevIndex = (nodes[i]->prev) ? nodeToIndex[nodes[i]->prev] : -1;
        int nextIndex = (nodes[i]->next) ? nodeToIndex[nodes[i]->next] : -1;
        int randIndex = (nodes[i]->rand) ? nodeToIndex[nodes[i]->rand] : -1;
        
        int dataLength = nodes[i]->data.length();
        
        out.write(reinterpret_cast<const char*>(&prevIndex), sizeof(prevIndex));
        out.write(reinterpret_cast<const char*>(&nextIndex), sizeof(nextIndex));
        out.write(reinterpret_cast<const char*>(&randIndex), sizeof(randIndex));
        out.write(reinterpret_cast<const char*>(&dataLength), sizeof(dataLength));
        
        out.write(nodes[i]->data.c_str(), dataLength);
    }
    
    out.close();
}

void deleteList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    ListNode* head = deserializeFromText("inlet.in");
    
    if (!head) {
        std::cout << "Empty list or file not found" << std::endl;
    } else {
        std::cout << "List successfully deserialized from text file" << std::endl;
    }
    
    serializeToBinary(head, "outlet.out");
    std::cout << "List serialized to binary file" << std::endl;
    
    deleteList(head);
    
    return 0;
}