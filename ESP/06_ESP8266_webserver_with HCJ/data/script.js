$=GeneratorJs()
        $.init()
        load("./calc.scss")
        append(app, "", "over")
        var rootElements = 'header,main'
        rootElements.split(",").forEach(e => {
            append(app, gen(e, e, e, e))
        })

        $("#main")[0].classList.add("container")
        append(main, gen(section, "calc", "", "calc"))
        append(header, gen(h1, "", "Calculator using GenetatorJs"), "over")

        append(calc, gen(div, "buttonGroup", "", `buttonGroup`),)

        var buttonList = 'C,1,2,3,4,5,6,7,8,9,0,.,+,-,*,/,(,),Del,='
        buttonList.split(",").forEach(b => {
            if (`0123456789.`.includes(b) == false) {

                append(buttonGroup, gen(button, b, b, `calcButton, ${b},operator`, { "onclick": "buttonAction(this)" }))
            }
            else {

                append(buttonGroup, gen(button, b, b, `calcButton, ${b}`, { "onclick": "buttonAction(this)" }))
            }
        })
        append(calc, gen(input, "expression", "", `expression`), "before")
        append(main, gen(div, "calcHistory", gen(h2, "", "History"), "calcHistory"))


        function buttonAction(e) {
            var val = e.innerText
            if (val == "=") {
                // var res = evalFunction(expression.value)
                var res = Function("return " + expression.value)()
                append(calcHistory, gen(span, "", expression.value + ' = ' + res, `calcHistory`, { "onclick": "reEvaluate(this)" }), "after")
                expression.value = res
                // append(result, res, "over")
            } else if (val == "C") {
                expression.value = ''
            } else if (val == "Del") {
                expression.value = expression.value.substring(0, expression.value.length - 1)
            }
            else {

                expression.value += val
            }



        }
        window.buttonAction = buttonAction


        function reEvaluate(e) {
            var exp = e.innerText.split("=")[0]
            expression.value = exp;

        }

        window.reEvaluate = reEvaluate


        function led(state="on"){                       
            url=`/led?state=${state}`
            xhr= new XMLHttpRequest(); 
            xhr.open("GET", url, true);            
            xhr.send();
        }

        append(main, gen(button, "LEDON", "LED", "led",{onclick:"led(on)"}))
        append(main, gen(button, "LEDOFF", "LED", "led",{onclick:"led(off)"}))