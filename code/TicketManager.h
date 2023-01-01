#include<mutex>
#include<iostream>
class TicketManager {
public:
    static TicketManager& getInstance();
    std::string serve() ;
private:
    int ticket;
    std::mutex _ticket;
    TicketManager(int n): ticket(n) {};
};
inline TicketManager&
TicketManager::getInstance() {
    static TicketManager TK(1000);
    return TK;
}
inline std::string 
TicketManager::serve() {
    std::lock_guard<std::mutex> _mu{_ticket};

}