function insect() {

    let args = window.location.search.substr(1).split('&');
    let user = '';
    for (let i = 0; i < args.length; i++) {
        let kv = args[i].split('=');
        if (kv.length == 2 && kv[0] == "user") {
            user = kv[1];
            break;
        }
    }

    function insect_detail(id) {
        $.post("http://192.168.1.3:8001/api/insect_detail", JSON.stringify({ 'id': id }), function (data, status) {
            console.log(data);
            let message = data.name;
            message += "\n活跃时间: " + data.active_time + "月";
            alert(message);
        });
    }

    let all_insect;
    let inputs = [];

    $.get("http://192.168.1.3:8001/api/all_insect", { 'user': user }, function (data, status) {
        let h1 = $("<h1>动森北半球虫类图鉴</h1>");
        $("#insect").append(h1);

        all_insect = data;
        console.log(all_insect);
        let table = $("<table border='1'></table>");
        let col = [];
        for (let i = 0; i < 5; i++) {
            let tr = $("<tr></tr>");
            col.push(tr);
            table.append(tr);
        }

        for (let i = 0; i < data.length; i++) {
            let input = $("<input type='checkbox' />");
            input.prop("checked", data[i][2]);
            inputs.push(input);
            let td = $("<td></td>");
            let p = $("<span></span>");
            p.text(data[i][1]);
            p.click(function () {
                insect_detail(data[i][0]);
            });
            td.append(p);
            let idx = (data[i][0] - 1) % col.length;
            col[idx].append(td);
            td.append(input);
        }
        $("#insect").append(table);

        let select_all = $("<input value='全选' type='submit'>");
        let select_all_checked = true;
        let button = $("<input value='提交' type='submit'>");
        $("#insect").append(select_all);
        $("#insect").append(button);

        select_all.click(function () {
            for (let i = 0; i < inputs.length; i++) {
                inputs[i].prop("checked", select_all_checked);
            }
            if (select_all_checked) {
                select_all.prop("value", "全不选");
                select_all_checked = false;
            } else {
                select_all.prop("value", "全选");
                select_all_checked = true;
            }
        });

        button.click(function () {
            let count = 0;
            let change_add = false;
            let change_del = false;
            let analysis_add = '';
            let analysis_del = '';
            for (let i = 0; i < inputs.length; i++) {
                count += inputs[i].prop('checked');
                if (all_insect[i][2] == false && inputs[i].prop('checked') == true) {
                    analysis_add += ' ' + all_insect[i][1];
                    change_add = true;
                } else if (all_insect[i][2] == true && inputs[i].prop('checked') == false) {
                    analysis_del += ' ' + all_insect[i][1];
                    change_del = true;
                }
                all_insect[i][2] = inputs[i].prop('checked');
            }
            let message = "总数: " + count + " / " + inputs.length
            if (change_add) {
                message += "\n捕捉了:" + analysis_add
            }
            if (change_del) {
                message += "\n取消了:" + analysis_del
            }
            alert(message);
            if (change_add == false && change_del == false) {
                return;
            }
            $.post("http://192.168.1.3:8001/api/my_insect", JSON.stringify({ "user": user, "data": JSON.stringify(all_insect) }),
                function (data, status) {
                    console.log(status);
                }
            );
        });
    });
}

insect();
