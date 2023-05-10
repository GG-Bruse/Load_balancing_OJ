let login=document.getElementById('login');
let register=document.getElementById('register');
let form_box=document.getElementsByClassName('form-box')[0];
let register_box=document.getElementsByClassName('register-box')[0];
let login_box=document.getElementsByClassName('login-box')[0];
// 去注册按钮点击事件
register.addEventListener('click',()=>{
    form_box.style.transform='translateX(80%)';
    login_box.classList.add('hidden');
    register_box.classList.remove('hidden');
})
// 去登录按钮点击事件
login.addEventListener('click',()=>{
    form_box.style.transform='translateX(0%)';
    register_box.classList.add('hidden');
    login_box.classList.remove('hidden');
})
//展开
var btnToggle = document.getElementById('btn-toggle');
var overlay = document.getElementById('overlay');

btnToggle.addEventListener('click', function() {
  if (overlay.style.display === 'block') {
    overlay.style.display = 'none'; // 如果弹出框已经显示，再次点击按钮时则隐藏弹出框
  } else {
    overlay.style.display = 'block'; // 否则显示弹出框
  }
});