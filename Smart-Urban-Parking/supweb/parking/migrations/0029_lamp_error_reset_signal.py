# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0028_ble_tag_timestamp'),
    ]

    operations = [
        migrations.AddField(
            model_name='lamp_error',
            name='reset_signal',
            field=models.BooleanField(default=False),
        ),
    ]
