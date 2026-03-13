$(function () {
    const showUrl = "http://127.0.0.1:8080/getTSSAData2";
    $("#saDataCO").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 1;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataHumidity").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 2;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataLight").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 3;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataLGP").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 4;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataMotion").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 5;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataSmoke").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 6;
                initChart(data, divId, buttonId);
            }
        });
    });
    $("#saDataTemp").click(function () {
        $.ajax({
            type: 'post',
            url: showUrl,
            dataType: 'json',
            success: function (data) {
                const divId = 'mainChart';
                const buttonId = 7;
                initChart(data, divId, buttonId);
            }
        });
    });

    let myChart;

    function initChart(data, divId, buttonId) {
        // 解决"There is a chart instance already initialized on the dom."问题。
        if (myChart != null && myChart != "" && myChart != undefined) {
            myChart.dispose();//销毁
        }
        myChart = echarts.init(document.getElementById(divId));
        myChart.setOption(getMyOption(data, buttonId));
    };

    function getMyOption(data, buttonId) {
        let option;
        switch (buttonId) {
            case 1:
                option = getMyBingTuOption(data);
                break;
            case 2:
                option = getMyZhuZhuangTuOption(data);
                break;
            case 3:
                option = getMyZheXianTuOption(data);
                break;
            case 4:
                option = getMyXiangShiTuOption(data);
                break;
            case 5:
                option = getMyXiangShiTuOption(data);
                break;
            case 6:
                option = getMyXiangShiTuOption(data);
                break;
            case 7:
                option = getMyXiangShiTuOption(data);
                break;
        }
        return option;
    };

    function getMyBingTuOption(data) {
        let dataY_t1 = 0, dataY_t2 = 0;
        for (let i = 1; i < data.length; i++) {
            if (data[i].type == '1') {
                dataY_t1 += parseInt(data[i].value);
            } else {
                dataY_t2 += parseInt(data[i].value);
            }
        }
        console.log("type1的value" + dataY_t1);
        console.log("type2的value" + dataY_t2);
        const option = {
            tooltip: {
                trigger: 'item'
            },
            legend: {
                top: '5%',
                left: 'center'
            },
            series: [
                {
                    name: 'Access From',
                    type: 'pie',
                    radius: ['40%', '70%'],
                    avoidLabelOverlap: false,
                    itemStyle: {
                        borderRadius: 10,
                        borderColor: '#fff',
                        borderWidth: 2
                    },
                    label: {
                        show: false,
                        position: 'center'
                    },
                    emphasis: {
                        label: {
                            show: true,
                            fontSize: '40',
                            fontWeight: 'bold'
                        }
                    },
                    labelLine: {
                        show: false
                    },
                    data: [
                        {value: dataY_t1, name: 'type1'},
                        {value: dataY_t2, name: 'type2'}
                    ]
                }
            ]
        };
        return option;
    };

    function getMyZhuZhuangTuOption(data) {
        const dataX = [], dataY = [];
        for (let i = 1, j = 0; i < data.length; i++, j++) {
            dataX[j] = data[i].id;
            dataY[j] = data[i].value;
        }
        const option = {
            xAxis: {
                type: 'category',
                data: dataX
            },
            yAxis: {
                type: 'value'
            },
            series: [
                {
                    data: dataY,
                    type: 'bar',
                    showBackground: true,
                    backgroundStyle: {
                        color: 'rgba(180, 180, 180, 0.2)'
                    }
                }
            ]
        };
        return option;
    };

    function getMyZheXianTuOption(data) {
        const dataX = [], dataY_t1 = [], dataY_t2 = [];
        for (let i = 1, j = 0; i < data.length; i++, j++) {
            dataX[j] = data[i].id;
            if (data[i].type == '1') {
                dataY_t1[j] = data[i].value;
            } else {
                dataY_t2[j] = data[i].value;
            }
        }
        // for (let i = 1, j = 0, k = 0; i < data.length; i++) {
        //     dataX[j] = data[i].id;
        //     if (data[i].type == '1'){
        //         dataY_t1[j] = data[i].value;
        //         j++;
        //     } else {
        //         dataY_t2[k] = data[i].value;
        //         k++;
        //     }
        // }
        console.log("x:" + dataX);
        console.log("type1的value" + dataY_t1);
        console.log("type2的value" + dataY_t2);
        const option = {
            title: {
                text: '折线图'
            },
            tooltip: {
                trigger: 'axis'
            },
            legend: {
                data: ['type1', 'type2']
            },
            grid: {
                left: '3%',
                right: '4%',
                bottom: '3%',
                containLabel: true
            },
            toolbox: {
                feature: {
                    saveAsImage: {}
                }
            },
            xAxis: {
                type: 'category',
                boundaryGap: false,
                data: dataX
            },
            yAxis: {
                type: 'value'
            },
            series: [
                {
                    name: 'type1',
                    type: 'line',
                    stack: 'Total',
                    data: dataY_t1
                },
                {
                    name: 'type2',
                    type: 'line',
                    stack: 'Total',
                    data: dataY_t2
                }
            ]
        };
        return option;
    };

    function getMyXiangShiTuOption(data) {
        const dataY_t1 = [], dataY_t2 = [];
        for (let i = 1, j = 0, k = 0; i < data.length; i++) {
            if (data[i].type == '1') {
                dataY_t1[j] = data[i].value;
                j++;
            } else {
                dataY_t2[k] = data[i].value;
                k++;
            }
        }
        console.log("type1的value" + dataY_t1);
        console.log("type2的value" + dataY_t2);
        const option = {
            title: [
                {
                    text: '箱式图',
                    left: 'center'
                },
                {
                    // UpperLimit = Q3 + 1.5IQR = 75%分位数 +（75%分位数 - 25%分位数）1.5
                    // LowerLimit = Q1 - 1.5IQR = 25%分位数 -（75%分位数 - 25%分位数）1.5
                    text: 'upper: Q3 + 1.5 * IQR \nlower: Q1 - 1.5 * IQR',
                    borderColor: '#999',
                    borderWidth: 1,
                    textStyle: {
                        fontWeight: 'normal',
                        fontSize: 14,
                        lineHeight: 20
                    },
                    left: '10%',
                    top: '90%'
                }
            ],
            dataset: [
                {
                    source: [
                        dataY_t1,
                        dataY_t2
                    ]
                },
                {
                    transform: {
                        type: 'boxplot',
                        config: {itemNameFormatter: 'type {value}'}
                    }
                },
                {
                    fromDatasetIndex: 1,
                    fromTransformResult: 1
                }
            ],
            tooltip: {
                trigger: 'item',
                axisPointer: {
                    type: 'shadow'
                }
            },
            grid: {
                left: '10%',
                right: '10%',
                bottom: '15%'
            },
            xAxis: {
                type: 'category',
                boundaryGap: true,
                nameGap: 30,
                splitArea: {
                    show: false
                },
                splitLine: {
                    show: false
                }
            },
            yAxis: {
                type: 'value',
                name: 'km/s minus 299,000',
                splitArea: {
                    show: true
                }
            },
            series: [
                {
                    name: 'boxplot',
                    type: 'boxplot',
                    datasetIndex: 1
                },
                {
                    name: 'outlier',
                    type: 'scatter',
                    datasetIndex: 2
                }
            ]
        };
        return option;
    };
})
