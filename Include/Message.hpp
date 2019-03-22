#pragma once

#include <functional>

template<typename Request, typename Response>
struct Message
{
    using RequestType = Request;
    using ResponseType = Response;
    Request request;
    std::function<void(const Response&)> commit;
    Message(Request request, std::function<void(const Response&)> commit) :
        request(std::move(request)),
        commit{commit}
    {}
};

template<typename Message, typename Request, typename Callback>
auto makeSharedMessage(Request&& p_request, Callback p_callback)
{
    return std::make_shared<Message>(std::forward<Request>(p_request), std::function<void(const typename Message::ResponseType&)>(std::move(p_callback)));
}
