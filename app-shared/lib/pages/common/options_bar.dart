import 'package:flutter/material.dart';

class OptionsBar extends StatelessWidget {
  final Color color;
  VoidCallback menuPressed;
  VoidCallback morePressed;

  OptionsBar(
      {@required this.color,
      @required this.menuPressed,
      @required this.morePressed});

  @override
  Widget build(BuildContext context) {
    var drawerButton = IconButton(
      icon: Icon(Icons.menu),
      color: color,
      padding: EdgeInsets.all(0),
      tooltip: 'Settings',
      onPressed: menuPressed,
    );

    var moreButton = Container(
        child: IconButton(
      icon: Icon(Icons.more_vert),
      color: color,
      padding: EdgeInsets.all(0),
      tooltip: 'More',
      onPressed: morePressed,
    ));

    return new Container(
        margin: new EdgeInsets.only(left: 12, right: 12),
        height: 56,
        child: new Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: <Widget>[drawerButton, moreButton],
        ));
  }
}
