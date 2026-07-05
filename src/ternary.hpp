#pragma once
struct ternary {
    enum data { tr, fl, ne };
    data val;
    ternary(data val) : val(val) {}
};
