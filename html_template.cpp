#include <string>
#include <cstring>

using namespace std;
extern const string htmlTemplate1;
extern const string htmlTemplate2;


const string htmlTemplate1{ u8R"xxx(<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
</head>
<body>
<script src="https://d3js.org/d3.v5.min.js"></script>
<script src="https://unpkg.com/@hpcc-js/wasm@0.3.11/dist/index.min.js"></script>
<script src="https://unpkg.com/d3-graphviz@3.0.5/build/d3-graphviz.js"></script>
<div>
<div id="title" style="text-align: center;"><h2>Title</h2></div>
<div id="graph" style="text-align: center;"></div>
</div>
<script>

var title = document.getElementById("title");
var dotIndex = 0;
var graphviz = d3.select("#graph").graphviz()
    .transition(function () {
        return d3.transition("main")
            .ease(d3.easeLinear)
            .delay(500)
            .duration(1500);
    })
    .logEvents(true)
    .on("initEnd", render);

function render() {
    var pair = dots[dotIndex];
    var dot = pair.dot;
    var t = pair.title;
    title.innerHTML = t;
    graphviz
        .renderDot(dot)
        .on("end", function () {
            // dotIndex = (dotIndex + 1) % dots.length;
            ++dotIndex;
            if(dotIndex<dots.length)
            {
                render();
            }
        });
}

var dots = [
)xxx" };

const string htmlTemplate2{ u8R"xxx(
];
</script>
</html>
)xxx" };

string generateHtml(const string &dotsArrayStr)
{
    return htmlTemplate1 + dotsArrayStr + htmlTemplate2;
}
