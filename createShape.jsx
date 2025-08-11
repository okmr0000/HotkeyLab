/**
 * After Effects ExtendScript
 * Activeなコンポジションに丸か四角のシェイプレイヤーを追加する
 */
(function () {
    var comp = app.project.activeItem;
    if (!(comp instanceof CompItem)) {
        alert("コンポジションを選択してください。");
        return;
    }

    // 形状選択
    var shapeType = prompt("作成する形状を入力してください（circle または square）", "circle");
    if (!shapeType) return;

    shapeType = shapeType.toLowerCase();
    if (shapeType !== "circle" && shapeType !== "square") {
        alert("circle か square を指定してください。");
        return;
    }

    app.beginUndoGroup("Create Shape");

    // シェイプレイヤーを追加
    var shapeLayer = comp.layers.addShape();
    shapeLayer.name = shapeType === "circle" ? "Circle" : "Square";

    var contents = shapeLayer.property("ADBE Root Vectors Group");
    var group = contents.addProperty("ADBE Vector Group");
    var groupContents = group.property("ADBE Vectors Group");

    // Path を追加（丸か四角を選択）
    var pathProperty;
    if (shapeType === "circle") {
        pathProperty = groupContents.addProperty("ADBE Vector Shape - Ellipse");
        // サイズ（直径）を設定
        pathProperty.property("ADBE Vector Ellipse Size").setValue([200, 200]);
    } else {
        pathProperty = groupContents.addProperty("ADBE Vector Shape - Rect");
        // 幅と高さを設定
        pathProperty.property("ADBE Vector Rect Size").setValue([200, 200]);
        // 角丸半径を 0（完全な四角形）に
        pathProperty.property("ADBE Vector Rect Roundness").setValue(0);
    }

    // 塗り (Fill) を追加
    var fill = groupContents.addProperty("ADBE Vector Graphic - Fill");
    fill.property("ADBE Vector Fill Color").setValue([1, 1, 1]); // 白

    // シェイプレイヤーをコンポジション中央に配置
    shapeLayer.property("ADBE Transform Group").property("ADBE Position").setValue([comp.width / 2, comp.height / 2]);

    app.endUndoGroup();
})();
