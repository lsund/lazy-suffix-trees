#include "match.h"

Match incomplete_match()
{
    Match res;
    res.done = false;
    res.success = false;
    return res;
}


Match successful_match()
{
    Match res;
    res.done = true;
    res.success = true;
    return res;
}


Match unsuccessful_match()
{
    Match res;
    res.done = true;
    res.success = false;
    return res;
}


Match make_match(bool success)
{
    Match res;
    res.done = true;
    res.success = success;
    return res;
}

