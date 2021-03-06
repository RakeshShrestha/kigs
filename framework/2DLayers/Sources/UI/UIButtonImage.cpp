#include "UI/UIButtonImage.h"
#include "TextureFileManager.h"
#include "Texture.h"
#include "NotificationCenter.h"
#include "AlphaMask.h"

//IMPLEMENT_AND_REGISTER_CLASS_INFO(UIButtonImage, UIButtonImage, 2DLayers);;
IMPLEMENT_CLASS_INFO(UIButtonImage)

UIButtonImage::UIButtonImage(const kstl::string& name,CLASS_NAME_TREE_ARG) : 
UIButton(name, PASS_CLASS_NAME_TREE_ARG)
,mUpTexture(*this,false,LABEL_AND_ID(UpTexture),"")
,mDownTexture(*this,false,LABEL_AND_ID(DownTexture),"")
,mOverTexture(*this,false,LABEL_AND_ID(OverTexture),"")
{
}

UIButtonImage::~UIButtonImage()
{
	mUpTexturePointer = NULL;
	mOverTexturePointer = NULL;
	mDownTexturePointer = NULL;
}

void UIButtonImage::InitModifiable()
{
	UIButton::InitModifiable();
	if(_isInit)
	{
		// load texture
		auto& textureManager = KigsCore::Singleton<TextureFileManager>();

		mUpTexturePointer =0;
		mOverTexturePointer = 0;
		mDownTexturePointer = 0;
		
		if(mUpTexture.const_ref() !="")
		{
			mUpTexturePointer = textureManager->GetTexture(mUpTexture.const_ref());
		}

		if (mOverTexture.const_ref() != "")
		{
			mOverTexturePointer = textureManager->GetTexture(mOverTexture.const_ref());
		}

		if (mDownTexture.const_ref() != "")
		{
			mDownTexturePointer = textureManager->GetTexture(mDownTexture.const_ref());
		}

		mTexturePointer = mUpTexturePointer;

		AutoSize();


		mUpTexture.changeNotificationLevel(Owner);
		mDownTexture.changeNotificationLevel(Owner);
		mOverTexture.changeNotificationLevel(Owner);
	}

}

void UIButtonImage::NotifyUpdate(const unsigned int labelid)
{
	if (labelid == mUpTexture.getLabelID() ||
		labelid == mDownTexture.getLabelID() ||
		labelid == mOverTexture.getLabelID())
	{
		ChangeTexture(mUpTexture.const_ref(), mOverTexture.const_ref(), mDownTexture.const_ref());
	}
	else
		UITexturedItem::NotifyUpdate(labelid);
}

void UIButtonImage::ChangeTexture(kstl::string _texturename, kstl::string _overtexturename, kstl::string _downtexturename)
{
	auto& textureManager = KigsCore::Singleton<TextureFileManager>();
	if (mUpTexturePointer)
		mUpTexturePointer = 0;

	if(_texturename != "")
	{
		mUpTexture = _texturename;
		mUpTexturePointer = textureManager->GetTexture(mUpTexture);
	}

	if(mOverTexturePointer && _overtexturename != "")
		mOverTexturePointer =0;

	if(_overtexturename != "")
	{
		mOverTexture = _overtexturename;
		mOverTexturePointer = textureManager->GetTexture(mOverTexture);
	}

	if(mDownTexturePointer && _downtexturename != "")
		mDownTexturePointer =0;

	if(_downtexturename != "")
	{
		mDownTexture = _downtexturename;
		mDownTexturePointer = textureManager->GetTexture(mDownTexture);
	}

	if(mAlphaMask)
	{
		this->removeItem((CMSP&)mAlphaMask);

		//Make new Mask
		mAlphaMask = KigsCore::GetInstanceOf(getName(), "AlphaMask");
		mAlphaMask->setValue(LABEL_TO_ID(Threshold),0.1);
		mAlphaMask->setValue(LABEL_TO_ID(TextureName),mUpTexture.c_str());
		this->addItem((CMSP&)mAlphaMask);
		mAlphaMask->Init();
	}
	//auto Size
	if (mUpTexturePointer && mAutoResize)
	{
		float width,height;

		mUpTexturePointer->GetSize(width,height);
		if(width != mSizeX || height != mSizeY)
		{
			mSizeX=width;
			mSizeY=height;
			mNeedUpdatePosition=true;
		}
	}

	ChangeState();
}


//-----------------------------------------------------------------------------------------------------
//ReloadTexture

void	UIButtonImage::ReloadTexture()
{
	if (mUpTexturePointer)
	{
		mUpTexturePointer->ReInit();
		mTexturePointer = mUpTexturePointer;
	}

	if (mOverTexturePointer)
	{
		mOverTexturePointer->ReInit();
	}

	if (mDownTexturePointer)
	{
		mDownTexturePointer->ReInit();
	}
}

void UIButtonImage::ChangeState()
{
	if (mIsEnabled) // down and mouse over only when enabled
	{
		if (mIsDown && mDownTexturePointer)
		{
			mTexturePointer = mDownTexturePointer;
			return;
		}
		else if (mIsMouseOver && mOverTexturePointer)
		{
			mTexturePointer = mOverTexturePointer;
			return;
		}
	}
	mTexturePointer = mUpTexturePointer;
}
