var user_id = -1;
var admistrator_id = -1;
var user_name = "";
var admi_name = "";

function CheckUser_Home() {//普通用户主页
    if (user_id > 0) {//已经登录，进行替换
        $(".navbar #btn-toggle").text(user_name);
        $(".navbar #btnx").text("退出").click(function() {
            // 清除 cookie
            document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
            // 跳转页面
            window.location.href = "/index.html";
          });
        return;
    }
    $.ajax({
        url: "/get_user_id",
        method: "Get",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        //async: false,
        success: function (data) {
            console.log(data);
            if (data.UserId > 0) {
                user_id = data.UserId;
                GetUsername();
                console.log(user_name);
                $(".navbar #btnx").text("退出").click(function() {
                    // 清除 cookie
                    document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
                    // 跳转页面
                    window.location.href = "/index.html";
                  });
                document.getElementById('btn-toggle').setAttribute('disabled', true);
                $(".navbar #btn-toggle").text(user_name);
            }
        },
    });
}

function CheckUser() {//普通用户
    if (user_id > 0) {
        $(".navbar #btn-toggle").text(user_name);
        $(".navbar #btnx").text("退出").click(function() {
            // 清除 cookie
            document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
            // 跳转页面
            window.location.href = "/index.html";
          });
        return;
    }
    $.ajax({
        url: "/get_user_id",
        method: "Get",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        //async: false,
        success: function (data) {
            if (data.UserId > 0) {
                user_id = data.UserId;
                GetUsername();
                console.log(user_name);
                $(".navbar #btnx").text("");
                document.getElementById('btn-toggle').setAttribute('disabled', true);
                $(".navbar #btn-toggle").text(user_name);
                $(".navbar #btnx").text("退出").click(function() {
                    // 清除 cookie
                    document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
                    // 跳转页面
                    window.location.href = "/index.html";
                  });
            } else {
                window.location.href = "/";
                alert("请先进行登录");
            }
        },
    });
}

function CheckAdministor_Home() {//管理员主页
    if (admistrator_id > 0) {
        $(".navbar #btn-toggle").text(admi_name);
        $(".navbar #btnx").text("退出").click(function() {
            // 清除 cookie
            document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
            // 跳转页面
            window.location.href = "/index.html";
          });
        return;
    }
    $.ajax({
        url: "/get_user_id",
        method: "Get",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        //async: false,
        success: function (data) {
            if (data.UserId > 0) {
                admistrator_id = data.UserId;
                GetAdinimistorname();
                $(".navbar #btnx").text(admi_name);
                $(".navbar #btn-toggle").text("退出").click(function() {
                    // 清除 cookie
                    document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
                    // 跳转页面
                    window.location.href = "/index.html";
                  });
            }
        },
    });
}

function CheckAdministor() {//管理员
    if (admistrator_id > 0) {
        $(".navbar #btn-toggle").text(admi_name);
        $(".navbar #btnx").text("退出").click(function() {
            // 清除 cookie
            document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
            // 跳转页面
            window.location.href = "/index.html";
          });
        return;
    }
    $.ajax({
        url: "/get_user_id",
        method: "Get",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        //async: false,
        success: function (data) {
            if (data.UserId > 0) {
                admistrator_id = data.UserId;
                GetAdinimistorname();
                $(".navbar #btnx").text(admi_name);
                $(".navbar #btn-toggle").text("退出").click(function() {
                    // 清除 cookie
                    document.cookie = "JSESSION=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";
                    // 跳转页面
                    window.location.href = "/index.html";
                  });
            } else {
                alert("请先进行登录");
                window.location.href = "/administrator.html";
            }
        },
    });
}

function GetUsername() {
    $.ajax({
        url: "/get_user_name",
        method: "Post",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        async: false,
        data: JSON.stringify({
            'UserId': user_id,
            'tableName': "user"
        }),

        success: function (data) {
            console.log(data);
            user_name = data.username;
            console.log(user_name);
        },
    });
}

function GetAdinimistorname() {
    $.ajax({
        url: "/get_user_name",
        method: "Post",
        dataType: 'json',
        contentType: 'application/json;charset=utf-8',
        //async : false ==> 禁止异步请求   chrome 有可能会禁止 ajax 的同步请求
        async: false,
        data: JSON.stringify({
            'UserId': admistrator_id,
            'tableName': "administrators"
        }),
        success: function (data) {
            admi_name = data.username;
            console.log(admi_name);
        },
    });
}
