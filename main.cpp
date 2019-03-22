#include "Board.hpp"
#include "Dispatcher.hpp"
#include "BoardPrinter.hpp"
#include "GetBoardMessage.hpp"

int main()
{
    Board board = FourPlayersBoardBuilder{}.build();

    Dispatcher dispatcher;

    //dispatcher.subscribe<GetBoardMessage>([&](std::shared_ptr<GetBoardMessage> msg){msg->commit(board);});
    dispatcher.subscribe(interestByReactOn<GetBoardMessage>().with([&](std::shared_ptr<GetBoardMessage> msg){msg->commit(board);}));

    BoardPrinter printer{dispatcher};


    std::cout << "reading state" << std::endl;
    printer.readState();
    std::cout << "read state" << std::endl;
    printer.print();
}
