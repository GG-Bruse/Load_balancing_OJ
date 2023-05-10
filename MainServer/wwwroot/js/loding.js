function activeButton(clickedButton) {
    const buttons = document.getElementsByClassName("sign");
    if (buttons[0] === clickedButton && buttons[0].disabled === false && buttons[1].disabled === false) {
        console.log(1);
        buttons[0].disabled = false;
        buttons[1].disabled = true;
    }
    else if (buttons[0] === clickedButton && buttons[0].disabled === false && buttons[1].disabled === true) {
        console.log(2);
        buttons[0].disabled = false;
        buttons[1].disabled = false;
    }
    else if (buttons[1] === clickedButton && buttons[0].disabled === false && buttons[1].disabled === false) {
        console.log(3);
        buttons[1].disabled = false;
        buttons[0].disabled = true;
    }
    else if (buttons[1] === clickedButton && buttons[0].disabled === true && buttons[1].disabled === false) {
        console.log(4);
        buttons[1].disabled = false;
        buttons[0].disabled = false;
    }
}