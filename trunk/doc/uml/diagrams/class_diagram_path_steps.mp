input metauml;

beginfig(1);

% class declaration (in alphabetic order)
AbstractClass.PathStepAttribute("PathStepAttribute")
  ("-type: ATTRIBUTE_TYPE")
  ("+PathStepAttribute(ATTRIBUTE_TYPE _type)",
   "+~PathStepAttribute()",
   "+print(OutputStream& dos) const=0: void",
   "+getType(): const ATTRIBUTE_TYPE",
   "+clone()=0: PathStepAttribute*");

Class.PathStepAttributePosition("PathStepAttributePosition")
  ("-position: unsigned")
  ("+PathStepAttributePosition(unsigned _position)",
   "+~PathStepAttributePosition()",
   "+print(OutputStream& dos) const: void",
   "+getPosition(): unsigned",
   "+clone(): PathStepAttributePosition*");

AbstractClass.PathStepExpression("PathStepExpression")
  ("#axis: AXIS_TYPE",
   "#attribute: PathStepAttribute*")
  ("+PathStepExpression(EXP_TYPE _type, AXIS_TYPE _axis, PathStepAttribute* _attribute)",
   "+~PathStepExpression()",
   "+print(OutputStream& dos) const=0: void",
   "+getAxisType(): AXIS_TYPE",
   "+setAxisType(AXIS_TYPE _axis): void",
   "+getNodeTestType(): NODETEST_TYPE",
   "+getAttribute(): PathStepAttribute*",
   "+setAttribute(PathStepAttribute* _attribute): void",
   "+getStepWeight(bool is_last_step): unsigned",
   "+isTagNodeTest(): bool",
   "+isStarNodeTest(): bool",
   "+isNodeNodeTest(): bool",
   "+isTextNodeTest(): bool",
   "+isDosNodeStep(): bool",
   "+isSyntacticallyEqualTo(PathStepExpression* ps): bool",
   "+isMatchingTag(TAG tag)=0: bool",
   "+hasAttribute(): bool",
   "+clone()=0: PathStepExpression*",
   "+cloneWithoutAttributes()=0: PathStepExpression*");

PathStepExpression.info.foreColor := (0.600000,0.600000,0.600000);

Class.PathStepNodeExpression("PathStepNodeExpression")
  ()
  ("+PathStepNodeExpression(AXIS_TYPE _axis)",
   "+PathStepNodeExpression(AXIS_TYPE _axis, PathStepAttribute* _attribute)",
   "+PathStepNodeExpression()",
   "+~PathStepNodeExpression()",
   "+print(OutputStream& dos) const: void",
   "+isMatchingTag(TAG tag): bool",
   "+clone(): PathStepNodeExpression*",
   "+cloneWithoutAttributes(): PathStepNodeExpression*");

Class.PathStepStarExpression("PathStepStarExpression")
  ()
  ("+PathStepStarExpression(AXIS_TYPE _axis)",
   "+PathStepStarExpression(AXIS_TYPE _axis, PathStepAttribute* _attribute)",
   "+PathStepStarExpression()",
   "+~PathStepStarExpression()",
   "+print(OutputStream& dos) const: void",
   "+isMatchingTag(TAG tag): bool",
   "+clone(): PathStepStarExpression*",
   "+cloneWithoutAttributes(): PathStepStarExpression*");

Class.PathStepTagExpression("PathStepTagExpression")
  ("-nt: TAG")
  ("+PathStepTagExpression(AXIS_TYPE _axis, const char* _tag)",
   "+PathStepTagExpression(const char* _tag)",
   "+PathStepTagExpression(AXIS_TYPE _axis, TAG _tag, PathStepAttribute* _attribute)",
   "+~PathStepTagExpression()",
   "+print(OutputStream& dos) const: void",
   "+getNodeTest(): TAG",
   "+isMatchingTag(TAG tag): bool",
   "+clone(): PathStepTagExpression*",
   "+cloneWithoutAttributes(): PathStepTagExpression*");

Class.PathStepTextExpression("PathStepTextExpression")
  ()
  ("+PathStepTextExpression(AXIS_TYPE _axis)",
   "+PathStepTextExpression(AXIS_TYPE _axis, PathStepAttribute* _attribute)",
   "+PathStepTextExpression()",
   "+~PathStepTextExpression()",
   "+print(OutputStream& dos) const: void",
   "+isMatchingTag(TAG tag): bool",
   "+clone(): PathStepTextExpression*",
   "+cloneWithoutAttributes(): PathStepTextExpression*");

leftToRight.bottom(40)(PathStepNodeExpression, PathStepExpression, PathStepTextExpression);
topToBottom.right(40)(PathStepNodeExpression, PathStepStarExpression);
topToBottom.midx(40)(PathStepExpression, PathStepAttribute, PathStepAttributePosition);
topToBottom.left(40)(PathStepTextExpression, PathStepTagExpression);

drawObjects(PathStepAttribute,
  PathStepAttributePosition,
  PathStepExpression,
  PathStepNodeExpression,
  PathStepStarExpression,
  PathStepTagExpression,
  PathStepTextExpression);

% link declarations
link(inheritance)(pathHorizontal(PathStepNodeExpression.e, PathStepExpression.left));
link(inheritance)(pathStepX(PathStepStarExpression.e, PathStepExpression.w, 20));
link(inheritance)(rpathStepX(PathStepExpression.e, PathStepTagExpression.w, 20));
link(inheritance)(pathHorizontal(PathStepTextExpression.w, PathStepExpression.right));
link(dependency)(pathVertical(PathStepExpression.s, PathStepAttribute.top));
item(iStereo)("<<use>>")(obj.w = .5[PathStepExpression.s, PathStepAttribute.n]);
link(inheritance)(pathVertical(PathStepAttributePosition.n, PathStepAttribute.bottom));

endfig;
end