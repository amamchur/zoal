#include "../misc/cmd_argument.hpp"
#include "../misc/cmd_line_parser.hpp"
#include "../misc/timer_freq_calculator.hpp"
#include "../misc/type_detector.hpp"

#include <cstdint>
#include <iostream>
#include <zoal/ct/type_list.hpp>
#include <zoal/data/vector.hpp>
#include <zoal/parse/type_parser.hpp>

using command_line_parser = zoal::misc::command_line_parser;

//cmd_args command_args;
uint8_t buffer[256]{0};
zoal::misc::cmd_argument *arg = zoal::misc::cmd_argument::from_buffer(buffer);

void parser_argument(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    if (e == zoal::misc::command_line_event::line_end) {
        p->callback(&command_line_parser::empty_callback);
        return;
    }

    arg = arg->prepare_next();

    zoal::misc::type_detector_v2 td;
    auto ts = td.detect(p->token_start(), p->token_end(), p->token_end());
    auto te = p->token_end();
    ptrdiff_t mem_used = (reinterpret_cast<uint8_t *>(arg) - buffer);
    ptrdiff_t left_mem = (ptrdiff_t)sizeof(buffer) - mem_used;
    ptrdiff_t mem_required = (te - ts + 1) + (ptrdiff_t)sizeof(zoal::misc::cmd_argument) * 2;
    if (left_mem <= mem_required) {
        std::cout << "Out of memory!" << std::endl;
        p->callback(&command_line_parser::empty_callback);
        return;
    }

    switch (td.type()) {
    case zoal::misc::value_type::string:
        arg->string(ts, te);
        break;
    case zoal::misc::value_type::date_time: {
        auto dt = zoal::parse::type_parser<zoal::data::date_time>::parse(ts);
        arg->set(zoal::misc::value_type::date_time, dt);
        break;
    }
    case zoal::misc::value_type::date: {
        auto dt = zoal::parse::type_parser<zoal::data::date_time>::parse_date(ts);
        arg->set(zoal::misc::value_type::date, dt);
        break;
    }
    case zoal::misc::value_type::time: {
        auto dt = zoal::parse::type_parser<zoal::data::date_time>::parse_time(ts);
        arg->set(zoal::misc::value_type::time, dt);
        break;
    }
    case zoal::misc::value_type::integer:
        arg->integer(ts, te);
        break;
    default:
        break;
    }
}

void cmd_select_callback(zoal::misc::command_line_machine *p, zoal::misc::command_line_event e) {
    auto ts = p->token_start();
    auto te = p->token_end();
    std::string str(ts, te);
    std::cout << str << std::endl;
    p->callback(&parser_argument);
}

char cmd[] = "test asdas asd as";

int main() {
    auto s = cmd;
    auto e = cmd + sizeof(cmd) - 1;
    command_line_parser cmd_parser(nullptr, 0);
    cmd_parser.callback(cmd_select_callback);
    cmd_parser.scan(s, e, e);

    std::cout << "argument: " << sizeof(zoal::misc::cmd_argument) << std::endl;

    arg = arg->prepare_next();

    auto args_size = reinterpret_cast<uint8_t *>(arg) - buffer;
    std::cout << "args_size: " << args_size << std::endl;

    auto iter = reinterpret_cast<zoal::misc::cmd_argument *>(buffer);
    while (iter) {
        switch (iter->type) {
        case zoal::misc::value_type::string:
            std::cout << "String: " << iter->string() << std::endl;
            break;
        case zoal::misc::value_type::date_time:
            break;
        case zoal::misc::value_type::date:
            break;
        case zoal::misc::value_type::time:
            break;
        case zoal::misc::value_type::integer:
            std::cout << "Integer: " << iter->integer() << std::endl;
            break;
        }
        iter = iter->next();
    }
    return 0;
}
