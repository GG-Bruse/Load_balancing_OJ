

var btN = document.getElementById('btnx');
var bac1 = document.getElementById('bac1');

btN.addEventListener('click', function() {
	console.log('Hello world!');
  if (bac1.style.display === 'block') {
    bac1.style.display = 'none'; // 如果弹出框已经显示，再次点击按钮时则隐藏弹出框
  } else {
    bac1.style.display = 'block'; // 否则显示弹出框
  }
});