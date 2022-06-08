import 'package:flutter/material.dart';
import 'package:percent_indicator/percent_indicator.dart';
import 'dart:io';
import 'dart:convert';
import 'package:shell/shell.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:flutter_pty/flutter_pty.dart';
import 'package:file_selector/file_selector.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'mslauncher',
      theme: ThemeData(
          primarySwatch: Colors.blue,
          fontFamily: 'GenSenRounded',
          appBarTheme: AppBarTheme(
            textTheme: ThemeData.light().textTheme.copyWith(
                  bodyText1: TextStyle(
                    fontFamily: 'NotoSans',
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                  ),
                ),
          )),
      home: const MyHomePage(title: 'MineCraft Server Launcher'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  double StatusPercent = 0;
  String StatusMessage = "Waiting";
  String commandresult = "";
  late final Pty pty;
  String ptyOutout = "";

  void init(String acommandresult) {
    setState(() {
      commandresult = acommandresult;
    });
  }

  void initState() {
    super.initState();

    pty = Pty.start(shellos);

    pty.output.cast<List<int>>().listen((text) {
      var a = utf8.decode(text, allowMalformed: false);
      ptyOutout += a;
      setState(() {});
    });

    pty.exitCode.then((code) {
      ptyOutout += 'the process exited with exit code $code';
    });
  }

  @override
  Widget build(BuildContext context) {
    final double deviceHeight = MediaQuery.of(context).size.height;
    final double deviceWidth = MediaQuery.of(context).size.width;
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
      ),
      body: Column(
        mainAxisSize: MainAxisSize.min,
        children: <Widget>[
          SpaceHeight(deviceHeight * 0.1),
          SizedBox(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.start,
              children: [
                SpaceWidth(deviceWidth * 0.1),
                Expanded(
                    child: SampleCard(
                        "サーバーをセットアップする！", deviceHeight * 0.3, context,
                        Type: 2)),
                SpaceWidth(deviceWidth * 0.1),
                Expanded(
                    child: SampleCard("サーバーを起動する！", deviceHeight * 0.3, context,
                        Type: 1)),
                SpaceWidth(deviceWidth * 0.1),
              ],
            ),
          ),
          SpaceHeight(deviceHeight * 0.1),
          SizedBox(
            width: deviceWidth * 0.8,
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                    child: StatusBar(deviceHeight * 0.3, StatusPercent,
                        StatusMessage, deviceHeight * 0.1)),
                SpaceWidth(deviceWidth * 0.1),
                Expanded(
                    child: Column(children: [
                  TextField(
                      decoration: InputDecoration(
                        border: OutlineInputBorder(),
                        labelText: 'コマンド入力',
                      ),
                      onSubmitted: (String value) {
                        onEntercommand(value);
                      }),
                  Container(
                    height: 200,
                    child: SingleChildScrollView(
                        child: Text("コマンド\n" + ptyOutout.toString())),
                  )
                ])),
                SpaceWidth(deviceWidth * 0.1),
              ],
            ),
          ),
        ],
      ),

      floatingActionButton: FloatingActionButton(
        onPressed: () => {
          Navigator.push(
              context, MaterialPageRoute(builder: (context) => SettingPage()))
        },
        tooltip: 'Increment',
        child: const Icon(Icons.settings),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }

  void onEntercommand(String text) {
    final carriageReturn = Platform.isWindows ? '\r\n' : '\n';
    pty.write(const Utf8Encoder().convert(text + carriageReturn));
  }
}

Widget SampleCard(String content, double height, BuildContext buildcontext,
    {int Type = 0}) {
  return Center(
      child: InkWell(
    onTap: () => {Server(Type, buildcontext)},
    child: Card(
      child: SizedBox(
        height: height,
        child: Center(child: Text(content)),
      ),
    ),
  ));
}

class SettingPage extends StatefulWidget {
  State createState() => SettingPageState();
  Widget build(context, watch) {
    return Scaffold();
  }
}

class SettingPageState extends State<SettingPage> {
  //0 enable_command_block 1 hardcore
  List<bool> booleanvalues = [false, false];
  Values values = Values();

  @override
  void Checkboxedit(int num) {
    booleanvalues[num] = !booleanvalues[num];
    setState(() {});
  }

  @override
  Widget build(context) {
    final double deviceHeight = MediaQuery.of(context).size.height;
    final double deviceWidth = MediaQuery.of(context).size.width;
    return Scaffold(
      appBar: AppBar(
        title: Text("設定"),
      ),
      body: SizedBox(
          child: Row(
        children: [
          SpaceWidth(deviceWidth * 0.1),
          SizedBox(
              width: deviceWidth * 0.9,
              child: SingleChildScrollView(
                child: Column(
                    mainAxisAlignment: MainAxisAlignment.spaceAround,
                    children: [
                      SpaceHeight(deviceHeight * 0.1),
                      Row(
                        children: [
                          ElevatedButton(
                              onPressed: FolderPressed, child: Text("フォルダ選択")),
                          Text(values.folderpath)
                        ],
                      ),
                      CheckBoxWidget(0, "コマンドブロックを有効化する。"),
                      CheckBoxWidget(1, "ハードコアを有効化"),
                      EditStringsButton(0, "ゲームモードの設定"),
                    ]),
              ))
        ],
      )),
    );
  }

  Widget CheckBoxWidget(int num, String text) {
    return Row(children: [
      Text(text),
      Checkbox(value: booleanvalues[num], onChanged: (a) => {Checkboxedit(num)})
    ]);
  }

  Widget EditStringsButton(int num, String title,
      {String fieldstring = "入力してください"}) {
    return Row(
      children: [
        Text(title),
        ElevatedButton(
            onPressed: () async => {
                  showDialog(
                      context: context,
                      builder: (context) {
                        return AlertDialog(
                          title: Text(fieldstring),
                          content: TextField(
                            decoration: InputDecoration(hintText: "ここに入力"),
                            onChanged: (a) => {EditString(a, num)},
                          ),
                          actions: <Widget>[
                            FlatButton(
                              color: Colors.white,
                              textColor: Colors.blue,
                              child: Text('OK'),
                              onPressed: () {
                                Navigator.pop(context);
                              },
                            ),
                          ],
                        );
                      })
                },
            child: Text("編集"))
      ],
    );
  }

  void EditString(String text, int num) {
    values.StringField[num] = text;
    setState(() {});
  }

  void FolderPressed() async {
    var file = await getDirectoryPath(
        initialDirectory: "", confirmButtonText: "フォルダ選択");
    if (file == null) {
      return;
    } else {
      setState(() {
        values.folderpath = file;
      });
    }
  }
}

Widget SpaceHeight(double height) {
  return SizedBox(
    height: height,
  );
}

Widget SpaceWidth(double width) {
  return SizedBox(
    width: width,
  );
}

Widget StatusBar(double size, double percent, String message, double radius) {
  return new CircularPercentIndicator(
    radius: radius,
    lineWidth: 10.0,
    percent: percent,
    center: new Text(message),
    progressColor: Colors.green,
  );
}

void Server(int Type, BuildContext context) async {
  //0 none 1 start 2 setup
  if (Type == 2) {
    print("fuckyou");
    bool isjavaa = await isjava();
    print(isjava().toString() + "adfasfd");
    if (!isjavaa) {
      return Alert(
          context,
          "javaがインストールされていません！インストールしてください。(JDK17)",
          () async => {
                await launchUrl(Uri.parse(
                    "https://www.oracle.com/java/technologies/javase/jdk17-archive-downloads.html")),
              }, () {
        return;
      });
    } else {}
  }
}

void SimpleAlert(
    BuildContext context, String content, void Function() callback_ok) {
  showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: Text("caution"),
          content: Text(content),
          actions: <Widget>[
            FlatButton(
              child: Text("ok"),
              onPressed: () {
                Navigator.of(context).pop();
              },
            )
          ],
        );
      });
}

void Alert(BuildContext context, String content, void Function() callback_ok,
    void Function() callback_cancel) {
  //show dialog
  showDialog(
    context: context,
    builder: (BuildContext context) {
      return AlertDialog(
        title: new Text("Caution"),
        content: new Text(content),
        actions: <Widget>[
          // usually buttons at the bottom of the dialog
          new FlatButton(
            child: new Text("Ok"),
            onPressed: () {
              callback_ok();
              Navigator.of(context).pop();
            },
          ),
          new FlatButton(
            child: new Text("Cancel"),
            onPressed: () {
              callback_cancel();
              Navigator.of(context).pop();
            },
          ),
        ],
      );
    },
  );
}

Future<bool> isjava() async {
  if (os() == 1) {
    var file = File("isjava.bat").openWrite();
    file.write("java -version");
    await file.close();
    var resulta = await Process.run("isjava.bat", []);
    var result = resulta.stderr;
    if (result.contains("Java(TM) SE Runtime Environment") &&
        result.contains("build 17")) {
      return true;
    } else {
      return false;
    }
  } else if (os() == 2 || os() == 3) {
    var file = File("isjava.sh").openWrite();
    file.write("java -version");
    await file.close();
    var resulta = await Process.run("isjava.sh", []);
    var result = resulta.stderr;
    if (result.contains("Java(TM) SE Runtime Environment") &&
        result.contains("build 17")) {
      return true;
    } else {
      return false;
    }
  } else if (os() == 4) {
    return true;
  } else {
    return false;
  }
}

int os() {
  if (Platform.isWindows) {
    return 1;
  } else if (Platform.isMacOS) {
    return 2;
  } else if (Platform.isLinux) {
    return 3;
  } else if (Platform.isAndroid) {
    return 4;
  } else {
    return 0;
  }
}

String get shellos {
  if (Platform.isMacOS || Platform.isLinux) {
    return 'bash';
    // return Platform.environment['SHELL'] ?? 'bash';
  }

  if (Platform.isWindows) {
    return 'cmd.exe';
  }

  return 'sh';
}

class Values {
  String folderpath = "";
  List<String> StringField = [
    "",
    "",
    "",
    "",
    "",
    ""
  ]; //0 gamemode 1 server.port 2 whielist 3 spawn.protection 4 ban users 5 ban ips
}

Values LoadServerSettings() {
  String folderpath = LoadUserSetting_ServerFolder();
  var data = ParseServerProperties();
  var values = Values();
  values.folderpath = folderpath;
  values.StringField[0] = data["gamemode"];
  values.StringField[1] = data["server.port"];
  values.StringField[2] = data["whitelist"];
  values.StringField[3] = data["spawn.protection"];
  values.StringField[4] = data["ban.users"];
  values.StringField[5] = data["ban.ips"];
  return values;
}

dynamic ParseServerProperties() {
  String FolderPath = LoadUserSetting_ServerFolder();
  var file = File(FolderPath + "\\server.properties");
  var jsonString = file.readAsStringSync();
  var a = jsonString.split("\n");
  var result = {"": ""};
  for (var b in a) {
    var c = b.split("=");
    result[c[0]] = c[1];
  }
  return result;
}

dynamic LoadUserSetting_ServerFolder() {
  var file = File("./user.setting");
  var jsonString = file.readAsStringSync();
  var a = jsonString.split("\n");
  var result = {"": ""};
  for (var b in a) {
    var c = b.split("=");
    result[c[0]] = c[1];
  }
  return result["serverfolder"];
}

void WriteServerProperties(Values NewData) {
  var data = ParseServerProperties();
  var file = File(data.folderpath + "\\server.properties");
  data["gamemode"] = NewData.StringField[0];
  data["server.port"] = NewData.StringField[1];
  data["whitelist"] = NewData.StringField[2];
  data["spawn.protection"] = NewData.StringField[3];
  data["ban.users"] = NewData.StringField[4];
  data["ban.ips"] = NewData.StringField[5];
}
